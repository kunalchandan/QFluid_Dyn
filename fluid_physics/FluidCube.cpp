#define IX(x, y, z) ((x) + (y) * this->size + (z) * this->size * this->size)

class FluidCube {
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
		this->diff = diffusion;
		this->visc = viscosity;
		this->dt   = dt;

		this->s = new float[N*N*N];
		this->density = new float[N*N*N];

		this->Vx = new float[N*N*N];
		this->Vy = new float[N*N*N];
		this->Vz = new float[N*N*N];

		this->Vx0 = new float[N*N*N];
		this->Vy0 = new float[N*N*N];
		this->Vz0 = new float[N*N*N];
	}

	void AddDensity(int x, int y, int z, float amount) {
		this->density[IX(x, y, z)] += amount;
	}

	void AddVelocity(int x, int y, int z, float amountX, float amountY, float amountZ) {
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
};