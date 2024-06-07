#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "camera.h"

// Structure to store model data
struct Mesh {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
};

// Global variables
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool firstMouse = true;
GLfloat lastX = 300, lastY = 300;
std::vector<Mesh> meshes;
GLfloat rotationAngle = 0.0f;

// Function to load an OBJ model
void loadOBJ(const std::string& filePath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error loading the model: " << importer.GetErrorString() << std::endl;
        return;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        Mesh mesh;
        const aiMesh* aiMesh = scene->mMeshes[i];

        for (unsigned int j = 0; j < aiMesh->mNumVertices; ++j) {
            mesh.vertices.push_back(aiMesh->mVertices[j].x);
            mesh.vertices.push_back(aiMesh->mVertices[j].y);
            mesh.vertices.push_back(aiMesh->mVertices[j].z);
        }

        for (unsigned int j = 0; j < aiMesh->mNumFaces; ++j) {
            const aiFace& face = aiMesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                mesh.indices.push_back(face.mIndices[k]);
            }
        }

        meshes.push_back(mesh);
    }
}

// Initialization function
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glutSetCursor(GLUT_CURSOR_NONE);
    loadOBJ("CT.obj");
}

// Rendering function
void render() {
    GLfloat currentFrame = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camera.GetZoom(), (GLfloat)600 / (GLfloat)600, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glm::vec3 cameraPos = camera.GetPosition();
    glm::vec3 cameraFront = camera.GetFront();
    glm::vec3 cameraUp = camera.GetUp();
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z,
        cameraPos.x + cameraFront.x, cameraPos.y + cameraFront.y, cameraPos.z + cameraFront.z,
        cameraUp.x, cameraUp.y, cameraUp.z);

    glRotatef(rotationAngle, 0.0, 1.0, 0.0);

    for (const auto& mesh : meshes) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, mesh.vertices.data());
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, mesh.indices.data());
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    glutSwapBuffers();
}

// Function to process keyboard input
void processInput(unsigned char key, int x, int y) {
    if (key == 27) // ESC key
        exit(0);
    GLfloat movementSpeed = 12.0f; // Adjust the movement speed as needed
    if (key == 'w')
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (key == 's')
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (key == 'a')
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (key == 'd')
        camera.ProcessKeyboard(RIGHT, deltaTime);

    glutPostRedisplay();
}

// Mouse movement callback
void mouseCallback(int x, int y) {
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    GLfloat xOffset = x - lastX;
    GLfloat yOffset = lastY - y; // Reversed since y-coordinates range from bottom to top
    lastX = 300; // Reset cursor position to center
    lastY = 300; // Reset cursor position to center

    camera.ProcessMouseMovement(xOffset, yOffset);

    // Reset the cursor to the center of the window
    glutWarpPointer(300, 300);

    glutPostRedisplay();
}

// Mouse motion callback with button pressed
void mouseMotion(int x, int y) {
    rotationAngle += (x - glutGet(GLUT_WINDOW_WIDTH) / 2) * 0.1;
    glutPostRedisplay();
}

// Mouse event callback
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        glutMotionFunc(mouseMotion);
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        glutMotionFunc(nullptr);
    }
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("COOKIE TOWN");

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    init();

    glutDisplayFunc(render);
    glutKeyboardFunc(processInput);
    glutPassiveMotionFunc(mouseCallback);

    glutMouseFunc(mouse);
    glutMotionFunc(nullptr);

    glutMainLoop();

    return 0;
}
