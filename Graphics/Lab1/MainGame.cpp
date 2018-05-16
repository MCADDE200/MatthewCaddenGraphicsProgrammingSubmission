#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
    Mesh* monkey();
	Mesh* glass();
	Mesh* hat();
	Mesh* watch();
	Audio* audioDevice();
	Texture* texture(); //load texture
	Texture* texture1(); //load texture
	Overlay* overlay(); //load texture
	Shader* shaderBlur();
	Shader* shaderToon();
	Shader* shaderRim();
	Shader* shaderToonRim();
	Shader* shaderLighting();
	Shader* shaderNormals();
	Shader* shaderExplode();
	Shader* shaderBlinnPhong();
}

MainGame::~MainGame() 
{
}

void MainGame::run()
{
	initialiseSystems(); 
	gameLoop();
}

void MainGame::initialiseSystems()
{
	_gameDisplay.initDisplay(); 

	texture.init("..\\res\\bricks.jpg"); //load texture
	texture1.init("..\\res\\water.jpg"); //load texture

	blurShader.initialise("..\\res\\shaderBlur.vert", "..\\res\\shaderBlur.frag");
	toonShader.initialise("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag");
	rimShader.initialise("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
	toonRimShader.initialise("..\\res\\shaderToonRim.vert", "..\\res\\shaderToonRim.frag");
	lightingShader.initialise("..\\res\\shaderLighting.vert", "..\\res\\shaderLighting.frag");
	blinnPhongShader.initialise("..\\res\\shaderBlinnPhong.vert", "..\\res\\shaderBlinnPhong.frag");
	normalsShader.initGeometryShaders("..\\res\\shaderNormal.vert", "..\\res\\shaderNormal.frag", "..\\res\\shaderNormal.geom");
	explodeShader.initGeometryShaders("..\\res\\shaderToonRim.vert", "..\\res\\shaderToonRim.frag", "..\\res\\shaderExplosion.geom");


	overlay.init("..\\res\\bricks.jpg");

	glass.loadModel("..\\res\\glass-a04.obj"); 
	monkey.loadModel("..\\res\\monkey3.obj");
	hat.loadModel("..\\res\\objCap.obj");
	watch.loadModel("..\\res\\Smartwatch.obj");
	
	myCamera.initCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;
	counter1 = 0.0f;

	changeShaders = false;
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processTheInput();
		drawTheGame();

	}
}

void MainGame::processTheInput()
{
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent)) //get and process events
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_KEYDOWN:
			switch (sdlEvent.key.keysym.sym)
			{
			case SDLK_SPACE:
				if (!changeShaders)
				{
					changeShaders = true;
				}
				else
				{
					changeShaders = false;
				}
				break;
			}
		}
	}
}


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		//audioDevice.setlistener(myCamera.getPos(), m1Pos); //add bool to mesh
		//playAudio(whistle, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
{
	
	ALint state; 

	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	/*
	Possible values of state
	AL_INITIAL
	AL_STOPPED
	AL_PLAYING
	AL_PAUSED
	*/
	if (AL_PLAYING != state)
	{
		//audioDevice.playSound(Source, pos);
	}
}


void MainGame::setupToonLighting()
{
	toonShader.inputVec3("lightDir", glm::vec3(0.5, 0.5, 0.5));
}

void MainGame::setupRimShader()
{
	rimShader.inputMat4("u_vm", myCamera.GetView());
	rimShader.inputMat4("u_pm", myCamera.GetProjection());
}

void MainGame::setupToonRimLighting()
{
	toonRimShader.inputVec3("lightDir", glm::vec3(0.5, 0.5, 0.5));
	toonRimShader.inputMat4("u_vm", myCamera.GetView());
	toonRimShader.inputMat4("u_pm", myCamera.GetProjection());
}

void MainGame::setupLighting()
{
	lightingShader.inputVec3("lightDir", glm::vec3(0.5, 0.5, 0.5)); 
	
	lightingShader.inputMat4("u_vm", myCamera.GetView());
	lightingShader.inputMat4("u_pm", myCamera.GetProjection());

	lightingShader.inputVec3("fogColor", glm::vec3(0.2, 0.2, 0.2));
	lightingShader.inputFloat("minDist", -5.0f);
	lightingShader.inputFloat("maxDist", 5.0f);
}

void MainGame::setupNormalsLighting()
{
	normalsShader.inputMat4("projection", myCamera.GetProjection());
	normalsShader.inputMat4("view", myCamera.GetView());
	normalsShader.inputMat4("model", transform.GetModel());
}

void MainGame::setupExplosionLighting()
{
	explodeShader.inputVec3("lightDir", glm::vec3(0.5, 0.5, 0.5));
	explodeShader.inputMat4("u_vm", myCamera.GetView());
	explodeShader.inputMat4("u_pm", myCamera.GetProjection());
	explodeShader.inputFloat("time", counter);
}

void MainGame::setupBlinnPhongLighting()
{
	blinnPhongShader.inputVec3("lightPos", glm::vec3(sinf(counter), 0.5, 0.0));
	blinnPhongShader.inputVec3("viewPos", myCamera.getPos());
}

void MainGame::blobEffect()
{
	GLuint blockIndex = glGetUniformBlockIndex(blurShader.getTheProgram(), "BlobSettings");

	GLint blockSize;
	glGetActiveUniformBlockiv(blurShader.getTheProgram(), blockIndex,
		GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize); //get information about blobsettings and save it in blockSize

	GLubyte * blockBuffer = (GLubyte *)malloc(blockSize); //allocates the requested memory and returns a pointer to it.

														  // Query for the offsets of each block variable
	const GLchar *names[] = { "InnerColor", "OuterColor",
		"RadiusInner", "RadiusOuter" };

	GLuint indices[4];
	glGetUniformIndices(blurShader.getTheProgram(), 4, names, indices); // glGetUniformIndices retrieves the indices of a number of uniforms within program

	GLint offset[4];
	glGetActiveUniformsiv(blurShader.getTheProgram(), 4, indices, GL_UNIFORM_OFFSET, offset); //Returns information about several active uniform variables for the specified program object

	GLfloat outerColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat innerColor[] = { 1.0f, 1.0f, 0.75f, 1.0f };

	GLfloat innerRadius = 0.0f, outerRadius = 3.0f;

	memcpy(blockBuffer + offset[0], innerColor,
		4 * sizeof(GLfloat)); //destination, source, no of bytes. 
	memcpy(blockBuffer + offset[1], outerColor,
		4 * sizeof(GLfloat));
	memcpy(blockBuffer + offset[2], &innerRadius,
		sizeof(GLfloat));
	memcpy(blockBuffer + offset[3], &outerRadius,
		sizeof(GLfloat));

	GLuint uboHandle;

	glGenBuffers(1, &uboHandle);

	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer,
		GL_DYNAMIC_DRAW); //creates and initializes a buffer object's data store - targer, size, data, usage

	glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uboHandle); // bind a buffer object to an indexed buffer target - trager, index, buffer
}

void MainGame::drawTheGame()
{
	texture.Bind(0);	
	
	counter1 = counter1 + 0.1f;
	_gameDisplay.clearDisplay(0.4f, 0.6f, 0.2f, 1.0f);

	transform.SetPos(glm::vec3(sinf(counter), 1.0, 0.0));
	transform.SetRot(glm::vec3(0.0, 0.0, 0.0));
	transform.SetScale(glm::vec3(0.8, 0.8, 0.8));
	if (!changeShaders) 
	{
		normalsShader.BindShader();
		setupNormalsLighting();
		normalsShader.Update(transform, myCamera);
	}
	else
	{
		rimShader.BindShader();
		setupRimShader();
		rimShader.Update(transform, myCamera);
	}

	glass.draw();
	glass.updateSphereData(*transform.GetPos(), 0.62f);
	

	transform.SetPos(glm::vec3(-sinf(counter), -1.0, -sinf(counter)*5));
	transform.SetRot(glm::vec3(0.0, counter*5, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));
	
	if (!changeShaders)
	{
		blinnPhongShader.BindShader();
		setupBlinnPhongLighting();
		blinnPhongShader.Update(transform, myCamera);
	}
	else
	{
		lightingShader.BindShader();
		setupLighting();
		lightingShader.inputFloat("zValue", monkey.getSpherePos().z);
		lightingShader.Update(transform, myCamera);
	}

	monkey.draw();
	monkey.updateSphereData(*transform.GetPos(), 0.62f);

	transform.SetPos(glm::vec3(4.0, 0.0, 0.0));
	transform.SetRot(glm::vec3(0.0, -sinf(counter), 0.0));
	transform.SetScale(glm::vec3(0.3, 0.3, 0.3));
	

	if (!changeShaders)
	{
		blurShader.BindShader();
		blobEffect();
		blurShader.Update(transform, myCamera);
	}
	else
	{
		explodeShader.BindShader();
		setupExplosionLighting();
		explodeShader.Update(transform, myCamera);
	}
	hat.draw();
	hat.updateSphereData(*transform.GetPos(), 0.62f);

	transform.SetPos(glm::vec3(-4, 0.0, 0.0));
	transform.SetRot(glm::vec3(0.0, 0.0, -sinf(counter)));
	transform.SetScale(glm::vec3(0.01, 0.01, 0.01));

	if (!changeShaders)
	{
		toonShader.BindShader();
		setupToonLighting();
		toonShader.Update(transform, myCamera);
	}
	else
	{
		toonRimShader.BindShader();
		setupToonRimLighting();
		toonRimShader.Update(transform, myCamera);
	}
	
	watch.draw();
	watch.updateSphereData(*transform.GetPos(), 0.62f);

	counter = counter + 0.01f;

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 