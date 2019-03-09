#include <algorithm>
#include <iostream>

class FluidCube {
public:
	int size;
	float dt;
	float diff;
	float visc;

	float *s;
	float *density;

	float *Vx, *Vy, *Vz;
	float *Vx0, *Vy0, *Vz0;
	
	FluidCube(int size, float diffusion, float viscosity, float dt) {
		int N = size;
		this->size = size;
		this->diff = diffusion;
		this->visc = viscosity;
		this->dt   = dt;

		this->s = new float[N*N*N]{};
		this->density = new float[N*N*N]{};

		this->Vx = new float[N*N*N]{};
		this->Vy = new float[N*N*N]{};
		this->Vz = new float[N*N*N]{};

		this->Vx0 = new float[N*N*N]{};
		this->Vy0 = new float[N*N*N]{};
		this->Vz0 = new float[N*N*N]{};
	}

	void AddDensity(int x, int y, int z, float amount) {
		int N = this->size;
		this->density[IX(x, y, z)] += amount;
	}

	void AddVelocity(int x, int y, int z, float amountX, float amountY, float amountZ) {
		int N = this->size;
		int i = IX(x, y, z);
		this->Vx[i] += amountX;
		this->Vy[i] += amountY;
		this->Vz[i] += amountZ;
	}

	~FluidCube() {
		delete[] s;
		delete[] density;

		delete[] Vx;
		delete[] Vy;
		delete[] Vz;
		
		delete[] Vx0;
		delete[] Vy0;
		delete[] Vz0;
	}
	
	
	int IX(int x, int y, int z) {
		int N = this->size;
		return (((x)+(y * N) + (z * N * N)));
	}




	void FluidCube_Step() {
		// Apply diffuse advect and project operations here
		// Iterations
		int i = 4;
		diffuse(1, Vx0, Vx, i);
		diffuse(2, Vy0, Vy, i);
		diffuse(3, Vz0, Vz, i);

		project(Vx0, Vy0, Vz0, Vx, Vy, i);

		advect(1, Vx, Vx0, Vx0, Vy0, Vz0, dt);
		advect(2, Vy, Vy0, Vx0, Vy0, Vz0, dt);
		advect(3, Vz, Vz0, Vx0, Vy0, Vz0, dt);

		project(Vx, Vy, Vz, Vx0, Vy0, i);

		diffuse(0, s, density, i);
		advect(0, density, s, Vx, Vy, Vz, dt);
	}

	void set_bound(int b, float *x) {
		int N = this->size;
		for (int j = 1; j < N - 1; j++) {
			for (int i = 1; i < N - 1; i++) {
				if (b == 3) {
					x[IX(i, j, 0)] = -x[IX(i, j, 1)];
					x[IX(i, j, N - 1)] = -x[IX(i, j, N - 2)];
				}
				else {
					x[IX(i, j, 0)] = x[IX(i, j, 1)];
					x[IX(i, j, N - 1)] = x[IX(i, j, N - 2)];
				}
			}
		}
		for (int k = 1; k < N - 1; k++) {
			for (int i = 1; i < N - 1; i++) {
				if (b == 2) {
					x[IX(i, 0, k)] = -x[IX(i, 1, k)];
					x[IX(i, N - 1, k)] = -x[IX(i, N - 2, k)];
				}
				else {
					x[IX(i, 0, k)] = x[IX(i, 1, k)];
					x[IX(i, N - 1, k)] = x[IX(i, N - 2, k)];
				}
			}
		}
		for (int k = 1; k < N - 1; k++) {
			for (int j = 1; j < N - 1; j++) {
				if (b == 1) {
					x[IX(0, j, k)] = -x[IX(1, j, k)];
					x[IX(N - 1, j, k)] = -x[IX(N - 2, j, k)];
				}
				else {
					x[IX(0, j, k)] = -x[IX(1, j, k)];
					x[IX(N - 1, j, k)] = -x[IX(N - 2, j, k)];
				}
			}
		}
		float avg = 1 / 3;
		{
			x[IX(0, 0, 0)] = avg * 
									 (x[IX(1, 0, 0)] 
									+ x[IX(0, 1, 0)] 
									+ x[IX(0, 0, 1)]);
		
			x[IX(0, N-1, 0)] = avg * 
									 (x[IX(1, N-1, 0)] 
									+ x[IX(0, N-2, 0)] 
									+ x[IX(0, N-1, 1)]);
		
			x[IX(0, 0, N-1)] = avg * 
									 (x[IX(1, 0, N-1)] 
									+ x[IX(0, 1, N-1)] 
									+ x[IX(0, 0, N)]);

			x[IX(0, N-1, N-1)] = avg * 
									 (x[IX(1, N-1, N-1)] 
									+ x[IX(0, N-2, N-1)] 
									+ x[IX(0, N-1, N-2)]);
		
			x[IX(N-1, 0, 0)] = avg * 
									 (x[IX(N-2, 0, 0)] 
									+ x[IX(N-1, 1, 0)] 
									+ x[IX(N-1, 0, 1)]);
		
			x[IX(N-1, N-1, 0)] = avg * 
									 (x[IX(N-2, N-1, 0)] 
									+ x[IX(N-1, N-2, 0)] 
									+ x[IX(N-1, N-1, 1)]);

			x[IX(N-1, 0, N-1)] = avg * 
									 (x[IX(N-2, 0, N-1)] 
									+ x[IX(N-1, 1, N-1)] 
									+ x[IX(N-1, 0, N-2)]);
		
			x[IX(N-1, N-1, N-1)] = avg * 
									 (x[IX(N-2, N-1, N-1)] 
									+ x[IX(N-1, N-2, N-1)] 
									+ x[IX(N-1, N-1, N-2)]);
		}
	}

	void linear_solve(int b, float *d, float *d0, float a, float c, int iter) {
		int N = this->size;
		float recip = 1.0 / c;
		for (int k = 0; k < iter; k++) {

			for (int m = 1; m < N - 1; m++) {
				for (int j = 1; j < N - 1; j++) {
					for (int i = 1; i < N - 1; i++) {
						d[IX(i, j, m)] =
									(d0[IX(i  ,j  ,m  )] +
								 (a*( d[IX(i+1,j  ,m  )]
									+ d[IX(i-1,j  ,m  )]
									+ d[IX(i  ,j+1,m  )]
									+ d[IX(i  ,j-1,m  )]
									+ d[IX(i  ,j  ,m+1)]
									+ d[IX(i  ,j  ,m-1)]
									))) * recip;
					}
				}
			}
			set_bound(b, d);
		}
	}

	// Were x and x0 swapped??
	void diffuse(int b, float *x, float *x0, int iter) {
		float diff = this->diff;
		float dt = this->dt;
		float N = this->size;
		float a = dt * diff * (N - 2) * (N - 2);
		linear_solve(b, x, x0, a, 1 + 6 * a, iter);
	}

	void project(float *vx, float *vy, float *vz, float *p, float *div, int iter) {
		int N = this->size;
		float recip = 1 / N;
		for (int k = 1; k < N - 1; k++) {
			for (int j = 1; j < N - 1; j++) {
				for (int i = 1; i < N - 1; i++) {
					div[IX(i, j, k)] = -0.5f*(
										 vx[IX(i+1,j  ,k  )]
										-vx[IX(i-1,j  ,k  )]
										+vy[IX(i  ,j+1,k  )]
										-vy[IX(i  ,j-1,k  )]
										+vz[IX(i  ,j  ,k+1)]
										-vz[IX(i  ,j  ,k-1)]
						)*N;
					p[IX(i, j, k)] = 0;
				}
			}
		}
		set_bound(0, div);
		set_bound(0, p);
		linear_solve(0, p, div, 1, 6, iter);

		for (int k = 1; k < N - 1; k++) {
			for (int j = 1; j < N - 1; j++) {
				for (int i = 1; i < N - 1; i++) {
					vx[IX(i, j, k)] -= 0.5f * (  p[IX(i+1,j  ,k  )]
												-p[IX(i-1,j  ,k  )])*N;
					vy[IX(i, j, k)] -= 0.5f * (  p[IX(i  ,j+1,k  )]
												-p[IX(i  ,j-1,k  )])*N;
					vz[IX(i, j, k)] -= 0.5f * (  p[IX(i  ,j  ,k+1)]
												-p[IX(i  ,j  ,k-1)])*N;
				}
			}
		}
		set_bound(1, vx);
		set_bound(2, vy);
		set_bound(3, vz);
	}

	void advect(int b, float *d, float *d0, float *vx, float *vy, float *vz, float dt) {
		int N = this->size;
		float _N = N;
		float i__[2], j__[2], k__[2];
		float s__[2], t__[2], u__[2];
		
		float temp[3];
		float x, y, z;
		float i_, j_, k_;
		int i, j, k;

		float dtx = dt * (N - 2);
		float dty = dt * (N - 2);
		float dtz = dt * (N - 2);
			
		for (k = 1, k_ = 1; k < N - 1; k++, k_++) {
			for (j = 1, j_ = 1; j < N - 1; j++, j_++) {
				for (i = 1, i_ = 1; i < N - 1; i++, i_++) {
					temp[0] = dtx * vx[IX(i, j, k)];
					temp[1] = dty * vy[IX(i, j, k)];
					temp[2] = dtz * vz[IX(i, j, k)];

					x = i_ - temp[0];
					y = j_ - temp[1];
					z = k_ - temp[2];
					
					{
						if (x < 0.5f)
							x = 0.5f;
						if (x > _N + 0.5)
							x = _N + 0.5f;
						i__[0] = floorf(x);
						i__[1] = i__[0] + 1.0f;

						if (y < 0.5f)
							y = 0.5f;
						if (y > _N + 0.5)
							y = _N + 0.5f;
						j__[0] = floorf(y);
						j__[1] = j__[0] + 1.0f;

						if (z < 0.5f)
							z = 0.5f;
						if (z > _N + 0.5)
							z = _N + 0.5f;
						k__[0] = floorf(z);
						k__[1] = k__[0] + 1.0f;
					}
					
					s__[1] = x - i__[0];
					s__[0] = 1.0f - s__[1];
					t__[1] = y - j__[0];
					t__[0] = 1.0f - t__[1];
					u__[1] = z - k__[0];
					u__[0] = 1.0f - u__[1];

					int i_i[2] = { i__[0], i__[1] };
					int j_i[2] = { j__[0], j__[1] };
					int k_i[2] = { k__[0], k__[1] };

					d[IX(i, j, k)] = 

						s__[0] * ( t__[0] * ( u__[0] * d0[IX(i_i[0], j_i[0], k_i[0])]
											+ u__[1] * d0[IX(i_i[0], j_i[0], k_i[1])] )
								+( t__[1] * ( u__[0] * d0[IX(i_i[0], j_i[1], k_i[0])]
											+ u__[1] * d0[IX(i_i[0], j_i[1], k_i[1])] )))
									
					   +s__[1] * ( t__[0] * ( u__[0] * d0[IX(i_i[1], j_i[0], k_i[0])]
											+ u__[1] * d0[IX(i_i[1], j_i[0], k_i[1])] )
								+( t__[1] * ( u__[0] * d0[IX(i_i[1], j_i[1], k_i[0])]
											+ u__[1] * d0[IX(i_i[1], j_i[1], k_i[1])] )));
				}
			}
		}
		set_bound(b, d);
	}
};