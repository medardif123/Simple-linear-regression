#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
float a;
float b;
int n;
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
//get the slope and the intercept of the line
void linearLine(float** xy) {
	float xmoy=0;
	float ymoy=0;
	for (int i = 0; i < n; i++) {
		xmoy += xy[0][i];
	}
	xmoy /= n;
	for (int i = 0; i < n; i++) {
		ymoy += xy[1][i];
	}
	ymoy /= n;
	float cov=0;
	for (int i = 0; i < n; i++) {
		cov += (xy[0][i] - xmoy) * (xy[1][i] - ymoy);
	}
	float var=0;
	for (int i = 0; i < n; i++) {
		var += (xy[0][i] - xmoy) * (xy[0][i] - xmoy);
	}
	a = cov / var;
	b = ymoy - a * xmoy;
}
int main() {
	float** xy;
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
	printf("%f\n", ymax);
	linearLine(xy);
	printf("a=%f\tb=%f", a, b);
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
		glVertex3f(-1.0,((b)/ymax)*2-1, 0.0);
		glVertex3f(1.0, ((a*xmax+b) /ymax)*2-1, 0.0);
		glEnd();
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
}