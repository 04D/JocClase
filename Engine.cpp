#include "Engine.h"
#include "Enemy.h"
// This is the constructor, called whenever a 'Game' object is init.
Engine::Engine() {
	// Get the screen resolution and create an SFML window and View
	//mides de la finestra
	viewSize.x = 1024;
	viewSize.y = 768;
	//construïm la finestra de joc.
	window.create(sf::VideoMode(viewSize.x, viewSize.y),
		"Joc heroi - enemic",
		sf::Style::Default);
}
// This is the destructor, called whenever a 'Game' object is destroyed.
Engine::~Engine() {
}

// Load and run the game 
int Engine::run() {
	sf::Clock clock;
	init();
	while (window.isOpen()) {
		updateInput();
		sf::Time dt = clock.restart();
		if (!gameover) {
			update(dt.asSeconds());
		}
		draw();
	}
	return 0;
}
void Engine::init() {
	//carregam les imatges del fons
	skyTexture.loadFromFile("Assets/graphics/sky.png");
	skySprite.setTexture(skyTexture);
	bgTexture.loadFromFile("Assets/graphics/bg.png");
	bgSprite.setTexture(bgTexture);
	// Load font
	headingFont.loadFromFile("Assets/fonts/SnackerComic.ttf");
	scoreFont.loadFromFile("Assets/fonts/arial.ttf");
	// Set Heading Text
	headingText.setFont(headingFont);
	headingText.setString("Tiny Bazooka");
	headingText.setCharacterSize(84);
	headingText.setFillColor(sf::Color::Red);
	sf::FloatRect headingbounds = headingText.getLocalBounds();
	headingText.setOrigin(headingbounds.width / 2, headingbounds.height / 2);
	headingText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));
	// Set Score Text
	// Tutorial Text
	tutorialText.setFont(scoreFont);
	tutorialText.setString("Press Down Arrow to Fire and Start Game, Up Arrow to Jump");
	tutorialText.setCharacterSize(35);
	tutorialText.setFillColor(sf::Color::Red);
	sf::FloatRect tutorialbounds = tutorialText.getLocalBounds();
	tutorialText.setOrigin(tutorialbounds.width / 2, tutorialbounds.height / 2);
	tutorialText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.20f));
	// Audio
	bgMusic.openFromFile("Assets/audio/bgMusic.ogg");
	bgMusic.play();
	//inicialitzem el jugador
	Robert.init("Assets/graphics/hero.png", sf::Vector2f(viewSize.x * 0.25f, viewSize.y * 0.5f), 200);
	//inicialitzem els nombre aleatoris.
	srand((int)time(0));
}
void Engine::updateInput() {
	sf::Event event;
	// while there are pending events...
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::KeyPressed) {//revisem les tecles premudes
			if (event.key.code == sf::Keyboard::Up) {//personatge sala
				Robert.jump(750.0f);
				}
	if (event.key.code == sf::Keyboard::Down) {
	if (gameover) {//si no joguem llavors engeguem el joc.
	gameover = false;
					reset();
				}
			}
			
		}
		//condicions per tancar el programa
			if (event.key.code == sf::Keyboard::Escape || event.type == sf::Event::Closed)
			window.close();
	}
}
void Engine::update(float dt) {
	//actualitzem la posició del personatge
	Robert.update(dt);
	//modifiquem el temps per sabre el que ha passat
	currentTime += dt;
	// Spawn Enemies
	if (currentTime > 2 + prevTime) {
		spawnEnemy();
		prevTime = currentTime;
	}
	// Update Enemies
	for(Enemy* e : Monstres) {
		e->update(dt);

	}
	// Update rockets
	// Check collision between Rocket and Enemies
}
void Engine::draw() {
	// netejem la pantalla
	window.clear(sf::Color::Red);
	//dibuixem elements
	window.draw(skySprite);
	window.draw(bgSprite);
	window.draw(Robert.getSprite());
	for (Enemy* e : Monstres) {
		window.draw(e->getSprite());

	}

	//mostrem text depenent si estem jugant o no
	if (gameover) {
		window.draw(headingText);
		window.draw(tutorialText);
	}
	else {
		window.draw(scoreText);
	}
	//enviem a la pantalla.
	window.display();
}
void Engine::reset() {
	score = 0;
	currentTime = 0.0f;
	prevTime = 0.0;
	scoreText.setString("Score: 0");
}

void Engine::spawnEnemy() {
	int randLoc = rand() % 3;
	sf::Vector2f enemyPos;
	float speed;
	switch (randLoc) {
	case 0: enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.75f); speed = 400;
		break;
	case 1: enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.60f); speed = 550;
		break;
	case 2: enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.40f); speed = 210;
		break;
	default: printf("incorrect y value \n"); break;
	}
	Enemy* enemy = new Enemy();
	enemy->init("Assets/graphics/enemy.png", enemyPos, speed);
	Monstres.push_back(enemy);
}