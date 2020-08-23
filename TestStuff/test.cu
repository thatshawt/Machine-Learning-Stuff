#include <stdio.h>
#include <vector>
#include "../Common/helper.h"
#include <thrust\host_vector.h>
#include <thrust\device_vector.h>

using namespace std;
using namespace thrust;

template <class size_t>
class Cuda2D {
private:
	uintptr_t* rowStart;
	int rowSize;
	vector<uintptr_t> rows;
	vector<std::vector<size_t>>* data;

	void updateRows() {
		rows.clear();
		for (int r = 0; r < data->size(); r++) {
			rows.push_back((uintptr_t)(data->at(r).data()));
			printf("r: %d, ", r);
			printf("row size: %d, ",rows.size());
			printf("val: %p, ", rows[r]);
		}
		printf("\n");
		/*
		printf("\n0,0: %p unsigned cast data[]\n", (uint64_t)data[0].data());
		printf("0,0: %p unsigned cast data->at()\n", (uint64_t)data->at(0).data());
		printf("0,0: %p data->at()\n", data->at(0).data());
		printf("0,0: %p data[]\n", data[0].data());
		printf("0,0: %p unsigned cast data.data()\n", (uint64_t)data->data());
		printf("0,0: %p data.data()\n", data->data());
		*/
		rowSize = rows.size();
		rowStart = rows.data();
	}
public:
	std::vector<size_t>* getRow(int r) {
		return &data->at(r);
	}

	void setData(std::vector<std::vector<size_t>>* bruh) {
		this->data = bruh;
		updateRows();
	}

	__forceinline__ __host__ __device__ size_t* safeAt(int r, int i) {
		if (r + 1 > rowSize) {
			printf("too big\n");
			return nullptr;
		}
		printf("wtf is happening rn bas\n");
		//   *(int*)(void*)(uintptr_t)(void*)ints.at(0).data();
		return ((size_t*)(*(rowStart + r)) + i);
	}
};
__global__
void doFunction(Cuda2D<int>* bruh) {
	printf("bruh");
	printf("size: %d\n", bruh->safeAt(0,0));
}

int main() {
	std::vector<int> bruhs;
	bruhs.push_back(1);
	bruhs.push_back(2);
	bruhs.push_back(3);

	int i = 0;
	printf("%d: %d\n",i, *(bruhs.data()+i));

	Cuda2D<int> v2;

	std::vector<std::vector<int>> ints;
	ints.push_back({1,2,3});
	ints.push_back({4,5,6});

	void* originalPtr = ints.at(0).data();
	uintptr_t theIntVersion = (uintptr_t)originalPtr;
	void* newPtr = (void*)theIntVersion;

	int allInOne = *(int*)(void*)(uintptr_t)(void*)ints.at(0).data();

	printf("0,1: %d\n", *(int*)originalPtr);
	printf("0,1: %d\n", *(int*)newPtr);
	printf("0,1: %d\n", allInOne);

	v2.setData(&ints);

	printf("0,1: %d\n", v2.safeAt(0,0));

	int intsSize = ints.size();

	cudaMallocManaged((void**)&v2, sizeof(Cuda2D<int>));
	cudaMallocManaged((void**)&ints, sizeof(std::vector<int>) * ints.size());
	//ints.size() changes after doing this for some reason

	for (int r = 0; r < intsSize; r++) {
		std::vector<int>* row = v2.getRow(r);
		printf("ints size: %p, row: %d, row size: %d\n", ints.size(), r, row->size());
		cudaMallocManaged((void**)row, row->size() * sizeof(int));
	}

	printf("ints size: %d\n", ints.size());

	doFunction <<<1, 1 >>> (&v2);

	cudaDeviceSynchronize();

	cudaFree(&v2);
	cudaFree(&ints);

	for (int r = 0; r < intsSize; r++) {
		std::vector<int>* row = v2.getRow(r);
		cudaFree(row);
	}

	getchar();

	return 1;
}