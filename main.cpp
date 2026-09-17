#include <glad/glad.h>         
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "sphereGenerate.h"
#include "gridGeneration.h"
#include "arcBall.h"
#include "mouseCallbacksGLFW.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "dashboard.h"
#include "Objects.h"
#include "PhysicsConstants.h"
#include "lightPathfinder.h"


int windowWidth = 1920;
int windowHeight = 1200;
float dashboardWidth = 500.0;

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    std::vector<float> sphereVertices;
    std::vector<unsigned int> sphereIndices;

    std::vector<float> gridVertices;
    std::vector<unsigned int> gridIndices;

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Shapiro Sim", NULL, NULL);
    if (window == NULL) {
        std::cout << "GLFW window couldn't load.";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    gladLoadGL();
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 1920, 1200);
    glClearColor(0.10f, 0.10f, 0.1f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    generateSphere(0.5f, 30, 50, sphereVertices, sphereIndices);
    generateGrid(20.0f, 400, gridVertices, gridIndices);

    Shader shaderProgram{ "default.vert", "default.frag" };

    VAO vao1;
    vao1.Bind();

    VBO vbo1{ sphereVertices };
    EBO ebo1{ sphereIndices};

    
    vao1.linkVBO(vbo1, 0);
    vao1.Unbind();
    vbo1.Unbind();
    ebo1.Unbind();

    Shader shaderProgramGrid{ "grid.vert", "grid.frag" };

    VAO vaoGrid;
    vaoGrid.Bind();

    VBO vboGrid{ gridVertices };
    EBO eboGrid{ gridIndices };

    vaoGrid.linkVBO(vboGrid, 0);
    vaoGrid.Unbind();
    vboGrid.Unbind();
    eboGrid.Unbind();

    Shader shaderFlatYellow{ "flat.vert", "flat.frag" };

    VAO vaoCurved;
    vaoCurved.Bind();
    VBO vboCurved{ 300*sizeof(glm::vec3)};
    vaoCurved.linkVBO(vboCurved, 0);
    vaoCurved.Unbind();
    vboCurved.Unbind();

    VAO vaoDashed;
    vaoDashed.Bind();
    VBO vboDashed{ 600 * sizeof(glm::vec3) };
    vaoDashed.linkVBO(vboDashed, 0);
    vaoDashed.Unbind();
    vboDashed.Unbind();


    glm::vec3 cameraPos = glm::vec3(0.0f, 3.0f, 10.0f);
    glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    

    glfwSetMouseButtonCallback(window, mouse_button_signal);
    glfwSetCursorPosCallback(window, cursor_moved_signal);
    glfwSetScrollCallback(window, scroll_signal);


    StartingPoint startPos(-10,3,4);
    IntermediaryObject interMass(solarMass, solarRadius);
    DestinationPoint destinationPos(10, 7, 8);

    Dashboard dashboard{ startPos,interMass,destinationPos };
    dashboard.init(window);

    


    while (!glfwWindowShouldClose(window)) {

        glViewport(dashboardWidth, 0, windowWidth - dashboardWidth, windowHeight);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderProgram.activateShader();

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (windowWidth - dashboardWidth) / 1200.0f, 0.1f, 100.0f);

        glm::vec3 cameraPos = arcballCamera::getPosition();
        glm::mat4 view = arcballCamera::getViewMatrix();

        glm::mat4 sphereModel = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), glm::vec3(0.2f, 1.0f, 0.1f));
        glm::mat4 sphereMVP = proj * view * sphereModel;

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "u_Model"), 1, GL_FALSE, glm::value_ptr(sphereModel));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.id, "u_MVP"), 1, GL_FALSE, glm::value_ptr(sphereMVP));
        glUniform3f(glGetUniformLocation(shaderProgram.id, "u_CamPos"), cameraPos.x, cameraPos.y, cameraPos.z);

        vao1.Bind();
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);


        shaderProgramGrid.activateShader();
        glm::mat4 gridModel = glm::mat4(1.0f);
        glm::mat4 gridMVP = proj * view * gridModel;

        double r_s = schwarzschildRadius(interMass);
        double interRadius = interMass.getRadius();
        bool isBlackHole = (interRadius <= r_s);

        double massKg = std::max(1e20, interMass.getMass());
        double logMass = std::log10(massKg);

        //most massive is 1e41, phoenix a*, more mass wouldn't make sense
        double objectMassScaled = std::clamp(((logMass - 20) / (42 - 20)) * 3.5 + 0.5, 0.3, 4.0);

        double compactness = interRadius / std::max(r_s, 1.0);
        double logRadius = std::log10(std::max(1.0, compactness));
        double scaledRadius = std::clamp(0.1 + logRadius * 0.35, 0.1, 4.0);
        
        glUniform1f(glGetUniformLocation(shaderProgramGrid.id, "u_MassScale"), (float)objectMassScaled);
        glUniform1i(glGetUniformLocation(shaderProgramGrid.id, "u_IsBlackHole"), isBlackHole);
        glUniform1f(glGetUniformLocation(shaderProgramGrid.id, "u_ScaledRadius"), (float)scaledRadius);


        glUniformMatrix4fv(glGetUniformLocation(shaderProgramGrid.id, "u_MVP"), 1, GL_FALSE, glm::value_ptr(gridMVP));
        glUniform1f(glGetUniformLocation(shaderProgramGrid.id, "u_MaxRadius"), 19.0f);

        vaoGrid.Bind();
        glDrawElements(GL_LINES, gridIndices.size(), GL_UNSIGNED_INT, 0);

        shaderFlatYellow.activateShader();
        vao1.Bind();

        glm::vec3 coordStart(startPos.getX(), startPos.getY(), startPos.getZ());
        glm::vec3 coordDest(destinationPos.getX(), destinationPos.getY(), destinationPos.getZ());

        //yellow color for the rays of light
        glUniform3f(glGetUniformLocation(shaderFlatYellow.id, "u_Color"), 1.0f, 0.85f, 0.1f);

        glm::mat4 startModel = glm::scale(glm::translate(glm::mat4(1.0f), coordStart), glm::vec3(0.25f));
        glm::mat4 startMVP = proj * view * startModel;
        glUniformMatrix4fv(glGetUniformLocation(shaderFlatYellow.id, "u_MVP"), 1, GL_FALSE, glm::value_ptr(startMVP));
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);

        glm::mat4 destModel = glm::scale(glm::translate(glm::mat4(1.0f), coordDest), glm::vec3(0.25f));
        glm::mat4 destMVP = proj * view * destModel;
        glUniformMatrix4fv(glGetUniformLocation(shaderFlatYellow.id, "u_MVP"), 1, GL_FALSE, glm::value_ptr(destMVP));
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);

        glm::mat4 lineMVP = proj * view * glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderFlatYellow.id, "u_MVP"), 1, GL_FALSE, glm::value_ptr(lineMVP));

        std::vector<glm::vec3> dashedPath = generateUnperturbedLight(coordStart, coordDest);
        vboDashed.update(dashedPath);

        glLineWidth(2.0f);
        glUniform3f(glGetUniformLocation(shaderFlatYellow.id, "u_Color"), 0.8f, 0.7f, 0.2f);
        vaoDashed.Bind();
        glDrawArrays(GL_LINES, 0, dashedPath.size());

        glViewport(0, 0, windowWidth, windowHeight);
        dashboard.renderDashboard(dashboardWidth,windowHeight);
        

        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }

    //delete stuff we no longer need
    vao1.Delete();
    vbo1.Delete();
    ebo1.Delete();

    vaoGrid.Delete();
    vboGrid.Delete();
    eboGrid.Delete();

    vaoCurved.Delete();
    vboCurved.Delete();

    vaoDashed.Delete();
    vboDashed.Delete();

    shaderProgramGrid.deleteShader();
    shaderProgram.deleteShader();
    shaderFlatYellow.deleteShader();


    dashboard.cleanup();
    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}



//#include <iostream>
//#include <iomanip>
//#include "Computations.h"
//
//int main() {
//	StartingPoint a{-120,50,7};
//	IntermediaryObject O{solarMass,solarRadius};
//	DestinationPoint b{190,21,30};
//	double ut = unperturbedTime(a, b);
//	double dt = shapiroTimeDelayFixed(a, O, b);
//	double tt = ut + dt;
//	double ip = impactParameterComputation(a, b);
//	double dtApproximation = shapiroTimeDelayApproximation(a, O, b);
//	double error = approximationError(dt, dtApproximation);
//
//	std::cout << std::fixed << std::setprecision(12);
//
//	std::cout << "Unperturbed time: " << ut << " seconds.(" << ut/60 << " minutes).\n";
//	std::cout << "Shapiro time delay: " << dt*1e6 << " microseconds.\n";
//	std::cout << "Total time: " << tt << " seconds.(" << tt / 60 << " minutes).\n";
//	std::cout << "Impact parameter(the distance at which the unperturbed light is at its closest to the mass): " << ip * 1e-9 <<
//		" million kilometers.\n";
//	std::cout << "Shapiro time delay(impact parameter approximation): " << dtApproximation * 1e6 << " microseconds.\n";
//
//	std::cout << std::fixed << std::setprecision(2);
//
//	std::cout << "Approximation error: " << error << "%.\n";
//	return 0;
//}