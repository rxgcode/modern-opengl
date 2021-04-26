#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\vec2.hpp>

#include <iostream>
#include <vector>

#include "utils.h"

#define PUSH_COORD(P) vertices.insert(vertices.end(), {(P).x, (P).y});
#define PRINT_VERTICES for (int i=0; (unsigned int)i<vertices.size(); i++) cout<<":::v"<<i<<" = "<<vertices[i]<<endl;

using namespace std;

const float PI = 3.1468;

// variables globales
GLuint VBO[4], VAO;
GLuint shader_program, sp;
GLFWwindow* window;
const int R = 5;
vector<float> vertices(0);
int count_gotita, count_ojo;
double ZOOM = 0.85;

void all_points_curve(glm::vec2 P, glm::vec2 c, float angle, int points){
    /* angle in radians!! */
    angle *= -1;
    float s = angle / points;
    for (int i=0; i<points; i++){
        glm::vec2 Pp;
        Pp.x =  1 * (P.x - c.x)*cos(s*i) + (P.y - c.y)*sin(s*i);
        Pp.y = -1 * (P.x - c.x)*sin(s*i) + (P.y - c.y)*cos(s*i);
        glm::mat4 trans =  glm::translate(glm::mat4(1), glm::vec3(c, 0));

        PUSH_COORD(trans*glm::vec4(Pp,0.0,1.0));
    }
}
void next_point_curve(glm::vec2 P, glm::vec2 c, float angle, int points, int i){
    angle *= -1;
    float s = angle / points;
    glm::vec2 Pp;
    Pp.x =  1 * (P.x - c.x)*cos(s*i) + (P.y - c.y)*sin(s*i);
    Pp.y = -1 * (P.x - c.x)*sin(s*i) + (P.y - c.y)*cos(s*i);
    glm::mat4 trans =  glm::translate(glm::mat4(1), glm::vec3(c, 0));
    PUSH_COORD(trans*glm::vec4(Pp,0.0,1.0));
}
void gen_vertices_gotita() {
    /* genera vertices de GOTA para ser dibujados con GL_TRIANGLES_STRIP */
    float r = 1.7;
    float rr = 3.3;

    PUSH_COORD(glm::vec2(0,R));
    int points = 250;
    for(int i=0; i<points; i++){
        next_point_curve(glm::vec2(0, R), glm::vec2(0, rr), 3.14, points, i);
        next_point_curve(glm::vec2(0, R), glm::vec2(0, 0), 3.14/2.0, points, i);
    }
    for(int i=0; i<points; i++){
        next_point_curve(glm::vec2(0, rr-r), glm::vec2(0, -r), -3.14, points, i);
        next_point_curve(glm::vec2(-R, 0), glm::vec2(0, 0), 3.14/2.0, points, i);
    }
}
void gen_vertices_ojo() {
    all_points_curve(glm::vec2(R,0), glm::vec2(0.0,0.0), 3.14 * 2, 600);
    all_points_curve(glm::vec2(2*R/3,0), glm::vec2(0.0,0.0), 3.14 * 2, 600);
    all_points_curve(glm::vec2(0.4,0), glm::vec2(0.0,0.0), 3.14 * 2, 600);

}
void coord_to_gl(float space){
    for(size_t i=0; i<vertices.size(); i+=1){
        vertices[i]*=space;
    }
}
void set_gl_clear_color(double currentTime){
    double slow = 5;
    double osc1 = abs(0.5 * sin(currentTime/slow));
    double osc2 = abs(0.2 * -sin(currentTime/slow));
    double osc3 = abs(0.2 * -sin(-currentTime/slow));

    double c = 1;

	glClearColor(
        osc1*c,
        osc2*c,
        osc3*c,
        0.0f);

	glClear(GL_COLOR_BUFFER_BIT);
}


void init(){
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(4, VBO);

    gen_vertices_ojo();
    coord_to_gl(1.0/(R)); // 1/R = 1/5 = 0.2
    count_ojo = vertices.size();
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // 3 circulos
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    vertices.clear();

    gen_vertices_gotita();
    coord_to_gl(1.0/R); // 1/R = 1/5 = 0.2
    count_gotita = vertices.size();
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]); // gota 1
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]); // gota 2
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]); // gota 3
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    vertices.clear();

    shader_program = Utils::createShaderProgram("vertex.glsl", "fragment.glsl");

    glUseProgram(shader_program);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glUniform2f(glGetUniformLocation(shader_program, "u_resolution"), (float) width, (float) height);
}

void display(double currentTime) {
    /* ---- SENTIDO (HORARIO: 1 | ANTIHORARIO: -1) ----*/
    int s = -1;
    /* ------------------------------------------------*/
    ZOOM = 0.9 + abs(sin(currentTime))*0.08;
    glm::mat4 zoom = glm::scale(glm::mat4(1.0), glm::vec3(ZOOM, ZOOM, 1));

    /* ------------------------------------------------*/

    set_gl_clear_color(currentTime);
    glm::mat4 mv, mov_gota;
    glm::vec4 color;
    glUniform1f(glGetUniformLocation(shader_program, "u_time"), (float) currentTime);

    // ------------------ OJO ------------------
    // circulo 1
    color = glm::vec4(0.5+(sin(currentTime)), 0.0, 0.0, 1.0);
    mv = glm::mat4(1.0);
    mv = glm::translate(mv, glm::vec3(0,0,0.5));
    mv *= zoom;

    glUniformMatrix4fv(glGetUniformLocation(shader_program, "mv_matrix"), 1, false, value_ptr(mv));
    glUniform4fv(glGetUniformLocation(shader_program, "color"), 1, value_ptr(color));
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN , 0, count_ojo/6);

    color = glm::vec4(0.35, 0.0, 0.0, 1.0);
    glUniform4fv(glGetUniformLocation(shader_program, "color"), 1, value_ptr(color));
    glPointSize(14.0);
    glDrawArrays(GL_POINTS, 0, count_ojo/6);

    // circulo 2
    color = glm::vec4(0.35, 0.0, 0.0, 1.0);
    glUniform4fv(glGetUniformLocation(shader_program, "color"), 1, value_ptr(color));
    glPointSize(14.0);
    glDrawArrays(GL_POINTS, count_ojo/6, count_ojo/6);

    // circulo 3
    color = glm::vec4(0.35, 0.0, 0.0, 1.0);
    glUniform4fv(glGetUniformLocation(shader_program, "color"), 1, value_ptr(color));
    double Z = 1 + abs(sin(currentTime))*0.4;
    mv *= glm::scale(glm::mat4(1.0), glm::vec3(Z*Z, Z*Z, 1));
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "mv_matrix"), 1, false, value_ptr(mv));
    glDrawArrays(GL_TRIANGLE_FAN, 2*count_ojo/6, count_ojo/6);


    // -------------------------------------------

    // ------------------ GOTAS ------------------

    // GOTA 1
    color = glm::vec4(0.35, 0.0, 0.0, 1.0);
    mv = glm::mat4(1.0);
    mv = glm::scale(mv, glm::vec3(0.125, 0.125, 0.125));
    mv = glm::translate(mv, glm::vec3(sin(s*(PI/3 + currentTime))*R, cos(s* (PI/3 + currentTime))*R, 0));
    mv = glm::rotate (mv, s*-1*(PI/3 + (float)currentTime), glm::vec3(0,0,1));
    mv = glm::rotate (mv, -3.14f/(2.0f*2.0f), glm::vec3(0,0,1));
    mv = glm::scale(mv, glm::vec3(1.2, 1.2, 1));

    Z = 0.9 + abs(sin(currentTime))*0.6;
    mv *= glm::scale(glm::mat4(1.0), glm::vec3(Z, Z, 1));

    glUniformMatrix4fv(glGetUniformLocation(shader_program, "mv_matrix"), 1, false, value_ptr(mv));
    glUniformMatrix4fv(glGetUniformLocation(sp, "mv_matrix"), 1, false, value_ptr(mv));

    glUniform4fv(glGetUniformLocation(shader_program, "color"), 1, value_ptr(color));

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, count_gotita/2);

    // GOTA 2
    mv = glm::mat4(1.0);
    mv = glm::scale(mv, glm::vec3(0.125, 0.125, 0.125));
    mv = glm::translate(mv, glm::vec3(sin(s*(-PI/3 + (float)currentTime))*R, cos(s*(-PI/3 +currentTime))*R, 0));
    mv = glm::rotate (mv, s*-1*(-PI/3 + (float)currentTime), glm::vec3(0,0,1));
    mv = glm::rotate (mv, -3.14f/(2.0f*2.0f), glm::vec3(0,0,1));
    mv = glm::scale(mv, glm::vec3(1.2, 1.2, 1));
    Z = 0.9 + abs(sin(currentTime))*0.6;
    mv *= glm::scale(glm::mat4(1.0), glm::vec3(Z, Z, 1));

    glUniformMatrix4fv(glGetUniformLocation(shader_program, "mv_matrix"), 1, false, value_ptr(mv));
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, count_gotita/2);

    // GOTA 3
    mv = glm::mat4(1.0);
    mv = glm::scale(mv, glm::vec3(0.125, 0.125, 0.125));
    mv = glm::translate(mv, glm::vec3(sin(s*(-PI + (float)currentTime))*R, cos(s*(-PI +currentTime))*R, 0));
    mv = glm::rotate (mv, s*-1 * (PI + (float)currentTime), glm::vec3(0,0,1));
    mv = glm::rotate (mv, -3.14f/(2.0f*2.0f), glm::vec3(0,0,1));
    mv = glm::scale(mv, glm::vec3(1.2, 1.2, 1));
    Z = 0.9 + abs(sin(currentTime))*0.6;
    mv *= glm::scale(glm::mat4(1.0), glm::vec3(Z, Z, 1));

    glUniformMatrix4fv(glGetUniformLocation(shader_program, "mv_matrix"), 1, false, value_ptr(mv));
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, count_gotita/2);

    // ---------------------------
}

int main( void ){
	if (!glfwInit()) exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 	// Resizable option.
	window = glfwCreateWindow(800, 800, "Sharingan", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) exit(EXIT_FAILURE);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwSwapInterval(1);
    init();

	while (!glfwWindowShouldClose(window)) {
		display(glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(4, VBO);
	glDeleteVertexArrays(1, &VAO);
    glfwDestroyWindow(window);
	glfwTerminate();

	return 0;

}
