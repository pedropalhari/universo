# Desenvolvimento

## Cheatsheet de funções
* `gluNewQuadric()`: cria um objeto OpenGL.
* `glutIdleFunc(idleFunc)`: função que roda entre os frames, passa a função por parâmetros.
* `glutDisplayFunc(draw)`: função que desenha o frame, passa a função por parâmetros.

## Carregamento de texturas

O OpenGL não tem binds para carregar texturass, então estou usando uma biblioteca para tratar BMPs (mais fácil).

A biblioteca pode ser encontrada em: https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/

Mergi ela em uma função para ajudar a transformar o BMP em uma estrutura entendível pelas APIs do OpenGL.

```cpp

void loadTextureIntoArray unsigned char data[256][256][3], std::string bmpName);

//No código
loadTextureIntoArray(moonTextureData, "moon.bmp");

//...
glGenTextures(1, &moonTex);
  glBindTexture(GL_TEXTURE_2D, moonTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 255, 255, 0, GL_RGB, GL_UNSIGNED_BYTE,
               (const GLvoid*)moonTextureData);

//...

//Na hora de desenhar a esfera
glBindTexture(GL_TEXTURE_2D, moonTex);
```

## Desenho das esferas

Criada uma função helper que desenha as esferas:

```cpp
void drawSphere(GLUquadric* object, GLuint textureCode, float rotation = 0,
                point Position = {x : 0, y : 0, z : 0}) 

//No código
drawSphere(moon, moonTex, zRotated * 90, (point){x : 10, y : 0, z : -2});
```

Assim fica fácil controlar o posicionamento das esferas a cada loop do OpenGL.


## Rodando

Comando para compilação:

`gcc main.cpp -lGL -lGLU -lglut -lm -lstdc++ && ./a.out`

Instalar opengl e lstdc++6-9 caso necessário.
