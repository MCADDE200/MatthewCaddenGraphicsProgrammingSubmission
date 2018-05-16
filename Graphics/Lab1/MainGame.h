#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"
#include "Overlay.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initialiseSystems();
	void processTheInput();
	void gameLoop();
	void drawTheGame();
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	void playAudio(unsigned int Source, glm::vec3 pos);
	void blobEffect();
	void setupToonLighting();
	void setupRimShader();
	void setupToonRimLighting();
	void setupLighting();
	void setupNormalsLighting();
	void setupExplosionLighting();
	void setupBlinnPhongLighting();

	Display _gameDisplay;
	GameState _gameState;
	Mesh monkey;
	Mesh glass;
	Mesh hat;
	Mesh watch;
	Texture texture;
	Texture texture1;
	Overlay overlay;
	Shader blurShader;
	Shader toonShader;
	Shader rimShader;
	Shader toonRimShader;
	Shader lightingShader;
	Shader normalsShader;
	Shader explodeShader;
	Shader blinnPhongShader;

	glm::mat4 modelView;

	Camera myCamera;
	Audio audioDevice;

	float counter;
	unsigned int whistle;
	unsigned int backGroundMusic;
	float counter1;
	bool changeShaders;
};

