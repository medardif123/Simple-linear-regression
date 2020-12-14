#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
float a;
float b;
int n;
float* w;
float** xy;
//read the data from the file 
float** readCsv() {
	FILE* file;
	file = fopen("cars.csv", "r");
	if (file == NULL) {
		printf("error while openig the file\n");
	}
	float** xy;
	xy = (float**)malloc(2 * sizeof(float*));
	for (int i = 0; i < 2; i++) {
		xy[i] = (float*)malloc(sizeof(float));
	}
	int xcol=0, ycol=0;
	float x;
	float y;
	while (fscanf(file, "%*d,%f,%f", &x, &y)!=EOF) {
		xy[0][xcol] = x;
		xy[1][ycol] = y;
		xcol++;
		ycol++;
		for (int i = 0; i < 2; i++) {
			xy[i] = (float*)realloc(xy[i],(xcol+1)*sizeof(float));
		}
	}
	xy =(float**)realloc(xy, 3 * sizeof(float*));
	xy[2] = (float*)malloc(sizeof(float));
	xy[2][0] = xcol;
	return xy;
}
float* gradient() {
	float* grad = (float*)malloc(2 * sizeof(float));
	grad[0] = 0;
	grad[1] = 0;
	for (int i = 0; i < n; i++) {
		grad[0] += (-xy[1][i] + w[0] + w[1] * xy[0][i]);
		grad[1] += (-xy[1][i] + w[0] + w[1] * xy[0][i])*xy[0][i];
	}
	grad[0] = (grad[0]*2)/n;
	grad[1] =(grad[1]*2)/n;
	printf("grad[0]=%f\n",grad[0]);
	printf("grad[1]=%f\n",grad[1]);
	return grad;
}
float scalarProduct(float* x,float* y,int size) {
	float prod=0;
	for (int i = 0; i < size; i++) {
		prod += x[i] * y[i];
	}
	return prod;
}
float* product(float* x, float alpha,int size) {
	
	for (int i = 0; i < size; i++) {
		x[i] = x[i] * alpha;
	}
	return x;
}
float* sum(float* x, float* y, int size) {
	
	for (int i = 0; i < size; i++) {
		x[i] = x[i] + y[i];
	}
	return x;
}

float empiricalError(float* w) {
	float MSE=0;
	for (int i = 0; i < n; i++) {
		MSE += (-xy[1][i] + w[0] + w[1] * xy[0][i]) * (-xy[1][i] + w[0] + w[1] * xy[0][i]);
	}
	MSE =MSE/n;
	return MSE;
}

void gradientDescent() {
	w = (float*)malloc(2 * sizeof(float));
	w[0] = 0, w[1] = 1;
	float epsilon = 0.001;
	float stepSize = 0.003;
	float* grad = gradient();
	//stepSize=armijoStep(grad, stepSize);
	while (scalarProduct(grad, grad, 2) > epsilon) {
		for (int i = 0; i < 2; i++) {
			w[i] = w[i] - stepSize * grad[i];
		}
		grad = gradient();
		//stepSize = armijoStep(grad, stepSize);
	}
}
int main() {
	xy = readCsv();
	n = xy[2][0];
	float xmax = 0;
	float ymax=0;
	for (int i = 0; i < n; i++) {
		if (xmax < xy[0][i])
			xmax = xy[0][i];
		if (ymax < xy[1][i])
			ymax = xy[1][i];
	}
	for (int i = 0; i < n; i++) {
		printf("x=%f\n", xy[0][i]);
		printf("y=%f\n", xy[1][i]);
	}
	gradientDescent();
	printf("error=%f\n", empiricalError(w));
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Simple Linear Regression", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glPointSize(7);
		glClearColor(0.0, 0.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_POINTS);
		for (int i = 0; i < n; i++) {
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f((xy[0][i]/xmax)*2-1, (xy[1][i] / ymax) * 2 - 1, 0.0);
		}
		glEnd();
		glLineWidth(4);
		glColor3f(0.7, 0.0, 0.1);
		glBegin(GL_LINES);
		glVertex3f(-1.0,((w[0])/ymax)*2-1, 0.0);
		glVertex3f(1.0, ((w[1]*xmax+b) /ymax)*2-1, 0.0);
		glEnd();
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
}