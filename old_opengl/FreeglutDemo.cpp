#include <GL/glut.h>


static int year = 0, day = 0;
void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glutWireSphere(1.0, 20, 16); /*draw sun */
    glRotatef((GLfloat)year, 0.0, 1.0, 0.0);

    glTranslatef(2.0, 0.0,0.0);
    glRotatef((GLfloat)day, 0.0, 1.0, 0.0);
    glutWireSphere(0.2, 10, 8); /*draw smaller planet */
    glPopMatrix();
    glutSwapBuffers();
}
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    //设定透视方式
    glMatrixMode(GL_PROJECTION);
    //载入单位矩阵
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);

    //设置模型矩阵
    // glMatrixMode(GL_MODELVIEW);
    //载入单位矩阵
    // glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'd':
        day = (day + 10) % 360;
        glutPostRedisplay();
        break;
    case 'D':
        day = (day - 10) % 360;
        glutPostRedisplay();
        break;
    case 'y':
        year = (year + 5) % 360;
        glutPostRedisplay();
        break;
    case 'Y':
        year = (year - 5) % 360;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();

    glutReshapeFunc(reshape);  
    glutDisplayFunc(display);
    

    //键盘回调函数
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}