#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace sf;
using namespace std;

struct Block {
	Sprite BlockS;
	Texture BlockT;
}Red[10], White[10], Blue[10], Yellow1[10], Yellow2[10];

struct Users {
	string name;
	int score;
}User[50];

RenderWindow window(VideoMode(700, 450), "Brick Breaker");

CircleShape ball(10);

int
confirmation = 0,
Transformation = 0,
options = 0,
Scorecnt = 0,
Blockscnt = 50,
Userscnt = 0,
Speedy = 150,
Speedx = 50,
x = 0,
y = 350;

float  deltatime = 0;

string Playerinput;

Vector2i Mouse_Postion;

Font font;

Sound BallHit, ButtonClick;
SoundBuffer Hit, BClick;

Music music;

Text
PlayerName,
ScoreText,
LeaderboardText,
entername;

Texture EndGameMenuTexture;
Sprite EndGameMenu, PlayerNameMenu, player;

void Score();
void moveOfplayer();
void Main_Menu();
void Settings_Menu();
void HighScore_Menu();
void Blocks();
void DrawBlocks();
void Movement();
void save();
void EndGame_Menu();
int input();
void Name(Event event);
bool CompareScore(Users& a, Users& b) {
	return a.score > b.score;
}

int main() {
	music.openFromFile("material/Fluffing a Duck.wav");
	music.play();

	Hit.loadFromFile("material/Ball_Hit.ogg");
	BallHit.setBuffer(Hit);

	BClick.loadFromFile("material/ButtonClick.ogg");
	ButtonClick.setBuffer(BClick);

	Texture PlayerTexture;
	PlayerTexture.loadFromFile("material/PLAYER.png");
	player.setTexture(PlayerTexture);
	player.setOrigin(70, 10);
	player.setPosition(350, 440);

	Texture BackgroundTexture;
	Sprite Background;
	BackgroundTexture.loadFromFile("material/BACKGROUND.jpg");
	Background.setTexture(BackgroundTexture);

	ball.setOrigin(5, 5);
	ball.setPosition(350, 225);

	font.loadFromFile("material/BebasNeue-Regular.ttf");
	ScoreText.setFont(font);
	LeaderboardText.setFont(font);

	Userscnt = input() - 1;

	Texture PlayerNameMenuTexture;
	PlayerNameMenuTexture.loadFromFile("material/PLAYERNAME MENU.jpg");
	PlayerNameMenu.setTexture(PlayerNameMenuTexture);
	PlayerName.setFont(font);
	Clock clock;

	Blocks();

	while (window.isOpen()) {
		Event event;
		deltatime = clock.restart().asSeconds();

		while (window.pollEvent(event)) {

			if (event.type == Event::Closed)
				window.close();
			if (options == 5) {
				Name(event);
			}
		}

		switch (options) {
		case 0:
			Main_Menu();
			break;

		case 1:
			if (Blockscnt == 0 || ball.getPosition().y >= 440) {
				User[Userscnt].score = Scorecnt;
				save();
				input();
				Blocks();
				ball.setPosition(350, 225);
				player.setPosition(350, 440);
				Speedy = 150;
				Speedx = 50;
				Userscnt++;
				Blockscnt = 50;
				Scorecnt = 0;
				options = 4;
			}
			moveOfplayer();
			Movement();
			window.clear();
			window.draw(Background);
			Score();
			DrawBlocks();
			window.draw(player);
			window.draw(ball);
			break;

		case 2:
			HighScore_Menu();
			break;

		case 3:
			Settings_Menu();
			break;

		case 4:
			EndGame_Menu();
			window.clear();
			window.draw(EndGameMenu);
			break;

		case 5:
			window.clear();
			window.draw(PlayerNameMenu);
			window.draw(PlayerName);
			window.draw(entername);
			break;
		}
		window.display();
	}
	return 0;
}

void Score() {
	ScoreText.setPosition(10, 2);
	ScoreText.setCharacterSize(22);
	ScoreText.setString("Score : " + to_string(Scorecnt));
	window.draw(ScoreText);
}

void Movement() {
	y = 350;
	if (Speedx < 0)
		x = -Speedx;
	else
		x = Speedx;
	int BallPosx = ball.getPosition().x;
	int Dir;
	if (ball.getPosition().y < 0) {
		Speedy = y;
	}
	if (ball.getPosition().x < 10) {
		Speedx = x;
	}
	if (ball.getPosition().x > 690) {
		Speedx = -x;
	}

	if (ball.getGlobalBounds().intersects(player.getGlobalBounds())) {
		Speedy = -y;
		x = 200;
		if (confirmation == 0)
			BallHit.play();

		int  PlayerPos = player.getPosition().x;
		Dir = BallPosx - PlayerPos;

		if (Dir > 35) {
			Speedx = x + 50;
		}
		else if (Dir > 0 && Dir < 35) {
			Speedx = x - 100;
		}
		else if (Dir == 0) {
			Speedx = 0;
		}
		else if (Dir <0 && Dir > -35) {
			Speedx = -x + 100;
		}
		else {
			Speedx = -x - 50;
		}

	}

	for (size_t i = 0; i < 10; i++) {
		//cyan bricks
		if (ball.getGlobalBounds().intersects(White[i].BlockS.getGlobalBounds())) {
			if (confirmation == 0)
				BallHit.play();
			Scorecnt += 10;
			Blockscnt--;
			White[i].BlockS.setScale(0, 0);
			Speedy = -Speedy;
			int Blockpos = White[i].BlockS.getPosition().x;
			Dir = BallPosx - Blockpos;
			if (Dir < 17)
				Speedx = -x;
			else
				Speedx = x;
		}
		//yellow bricks
		if (ball.getGlobalBounds().intersects(Yellow1[i].BlockS.getGlobalBounds())) {
			if (confirmation == 0)
				BallHit.play();
			Scorecnt += 10;
			Blockscnt--;
			Yellow1[i].BlockS.setScale(0, 0);
			Speedy = -Speedy;
			int Blockpos = Yellow1[i].BlockS.getPosition().x;
			Dir = BallPosx - Blockpos;
			if (Dir < 17)
				Speedx = -x;
			else
				Speedx = x;
		}
		if (ball.getGlobalBounds().intersects(Yellow2[i].BlockS.getGlobalBounds())) {
			if (confirmation == 0)
				BallHit.play();
			Scorecnt += 10;
			Blockscnt--;
			Yellow2[i].BlockS.setScale(0, 0);
			Speedy = -Speedy;
			int Blockpos = Yellow2[i].BlockS.getPosition().x;
			Dir = BallPosx - Blockpos;
			if (Dir < 17)
				Speedx = -x;
			else
				Speedx = x;
		}
		//red bricks
		if (ball.getGlobalBounds().intersects(Red[i].BlockS.getGlobalBounds())) {
			if (confirmation == 0)
				BallHit.play();
			Scorecnt += 10;
			Blockscnt--;
			Red[i].BlockS.setScale(0, 0);
			Speedy = -Speedy;
			int Blockpos = Red[i].BlockS.getPosition().x;
			Dir = BallPosx - Blockpos;
			if (Dir < 17)
				Speedx = -x;
			else
				Speedx = x;
		}
		// blue bricks
		if (ball.getGlobalBounds().intersects(Blue[i].BlockS.getGlobalBounds())) {
			if (confirmation == 0)
				BallHit.play();
			Scorecnt += 10;
			Blockscnt--;
			Blue[i].BlockS.setScale(0, 0);
			Speedy = -Speedy;
			int Blockpos = Blue[i].BlockS.getPosition().x;
			Dir = BallPosx - Blockpos;
			if (Dir < 17)
				Speedx = -x;
			else
				Speedx = x;
		}
	}

	ball.move(Speedx * deltatime, Speedy * deltatime);
}

void Main_Menu() {
	Texture MainMenuTexture;
	Sprite MainMenu;
	MainMenuTexture.loadFromFile("material/MAIN MENU.jpg");
	MainMenu.setTexture(MainMenuTexture);

	window.clear();
	window.draw(MainMenu);

	if (Mouse::isButtonPressed(Mouse::Left)) {
		Mouse_Postion = Mouse::getPosition(window);
		if (Mouse_Postion.x > 311 && Mouse_Postion.x < 388 && Mouse_Postion.y > 152 && Mouse_Postion.y < 174) {
			options = 5;
			Playerinput = "";
			window.clear();
			if (confirmation == 0)
				ButtonClick.play();
		}
		if (Mouse_Postion.x > 284 && Mouse_Postion.x < 420 && Mouse_Postion.y > 201 && Mouse_Postion.y < 223) {
			options = 2;
			if (confirmation == 0)
				ButtonClick.play();
		}
		if (Mouse_Postion.x > 290 && Mouse_Postion.x < 416 && Mouse_Postion.y > 252 && Mouse_Postion.y < 273) {
			options = 3;
			if (confirmation == 0)
				ButtonClick.play();
		}
		if (Mouse_Postion.x > 318 && Mouse_Postion.x < 384 && Mouse_Postion.y > 304 && Mouse_Postion.y < 327) {

			window.close();
		}
	}
}

void HighScore_Menu() {
	Texture HighScoreMenuTexture;
	Sprite HighScoreMenu;
	HighScoreMenuTexture.loadFromFile("material/HIGH SCORE MENU.jpg");
	HighScoreMenu.setTexture(HighScoreMenuTexture);

	window.clear();
	window.draw(HighScoreMenu);
	LeaderboardText.setCharacterSize(30);
	LeaderboardText.setFillColor(Color(92, 193, 101));

	if (Mouse::isButtonPressed(Mouse::Left)) {
		Mouse_Postion = Mouse::getPosition(window);
		if (Mouse_Postion.x > 308 && Mouse_Postion.x < 400 && Mouse_Postion.y > 338 && Mouse_Postion.y < 361) {
			options = 0;
			if (confirmation == 0)
				ButtonClick.play();

		}
	}
	sort(User, User + Userscnt, CompareScore);
	if (Userscnt < 6) {
		for (int i = 0; i < Userscnt; i++)
		{
			LeaderboardText.setPosition(275, 130 + (i * 30));
			LeaderboardText.setString(User[i].name + ": " + to_string(User[i].score));
			window.draw(LeaderboardText);

		}
	}

	else {
		for (int i = 0; i < 6; i++)
		{
			LeaderboardText.setPosition(275, 130 + (i * 30));
			LeaderboardText.setString(User[i].name + ": " + to_string(User[i].score));
			window.draw(LeaderboardText);

		}
	}
}

void Settings_Menu() {
	Texture SettingsMenuTexture;
	Sprite SettingsMenu;
	SettingsMenuTexture.loadFromFile("material/SETTINGS MENU.jpg");
	SettingsMenu.setTexture(SettingsMenuTexture);

	Texture MuteTexture, SpeakersTexture;
	Sprite Mute, Speakers;
	MuteTexture.loadFromFile("material/MUTE.png");
	Mute.setTexture(MuteTexture);
	SpeakersTexture.loadFromFile("material/SPEAKERS.png");
	Speakers.setTexture(SpeakersTexture);
	Speakers.setPosition(360, 176);
	Mute.setPosition(360, 176);

	RectangleShape mute(Vector2f(100, 4));
	mute.setOrigin(50, 2);
	mute.setFillColor(Color(106, 154, 106));
	mute.setOutlineColor(Color::Black);
	mute.setOutlineThickness(1);
	mute.setRotation(30);
	mute.setPosition(294, 183.5);

	window.clear();
	window.draw(SettingsMenu);
	Event SoundSystem;
	while (window.pollEvent(SoundSystem)) {
		if (Mouse::isButtonPressed(Mouse::Left)) {
			Mouse_Postion = Mouse::getPosition(window);
			if (Mouse_Postion.x > 244 && Mouse_Postion.x < 344 && Mouse_Postion.y > 171 && Mouse_Postion.y < 196
				|| Mouse_Postion.x > 359 && Mouse_Postion.x < 381 && Mouse_Postion.y > 175 && Mouse_Postion.y < 194) {
				Transformation++;
				if (confirmation == 0)
					ButtonClick.play();

				if (confirmation == 1)
					music.play();
			}
			if (Mouse_Postion.x > 308 && Mouse_Postion.x < 400 && Mouse_Postion.y > 279 && Mouse_Postion.y < 302) {
				options = 0;
				if (confirmation == 0)
					ButtonClick.play();
			}
		}
	}

	if (Transformation % 2 == 0) {
		confirmation = 0;
	}
	else if (Transformation % 2 != 0) {
		confirmation = 1;
	}

	switch (confirmation) {
	case 0:
		Mute.setScale(0, 0);
		break;

	case 1:
		music.stop();
		Speakers.setScale(0, 0);
		window.draw(mute);
		break;
	}

	window.draw(Speakers);
	window.draw(Mute);
}

void moveOfplayer() {
	if (Keyboard::isKeyPressed(Keyboard::Key::Left) && player.getPosition().x > 70) {
		player.move(-500 * deltatime, 0);
	}
	if (Keyboard::isKeyPressed(Keyboard::Key::Right) && player.getPosition().x < 630) {
		player.move(500 * deltatime, 0);
	}
}

void Blocks() {
	// White bricks;
	for (size_t i = 0; i < 10; i++) {
		White[i].BlockT.loadFromFile("material/White.jpg");
		White[i].BlockS.setTexture(White[i].BlockT);
		White[i].BlockS.setPosition(10 + (i * 70), 30);
		White[i].BlockS.setScale(1.5, 1.5);
	}
	//yellow bricks
	for (size_t i = 0; i < 10; i++) {
		Yellow1[i].BlockT.loadFromFile("material/Yellow.jpg");
		Yellow1[i].BlockS.setTexture(Yellow1[i].BlockT);
		Yellow1[i].BlockS.setPosition(10 + (i * 70), 60);
		Yellow1[i].BlockS.setScale(1.5, 1.5);
	}
	//yellow bricks
	for (size_t i = 0; i < 10; i++) {
		Yellow2[i].BlockT.loadFromFile("material/Yellow.jpg");
		Yellow2[i].BlockS.setTexture(Yellow2[i].BlockT);
		Yellow2[i].BlockS.setPosition(10 + (i * 70), 120);
		Yellow2[i].BlockS.setScale(1.5, 1.5);
	}
	// red bricks
	for (size_t i = 0; i < 10; i++) {
		Red[i].BlockT.loadFromFile("material/Red.jpg");
		Red[i].BlockS.setTexture(Red[i].BlockT);
		Red[i].BlockS.setPosition(10 + (i * 70), 90);
		Red[i].BlockS.setScale(1.5, 1.5);
	}
	//blue bricks 
	for (size_t i = 0; i < 10; i++) {
		Blue[i].BlockT.loadFromFile("material/Blue.jpg");
		Blue[i].BlockS.setTexture(Blue[i].BlockT);
		Blue[i].BlockS.setPosition(10 + (i * 70), 150);
		Blue[i].BlockS.setScale(1.5, 1.5);
	}
}

void DrawBlocks() {
	//cyan bricks
	for (size_t i = 0; i < 10; i++) {
		window.draw(White[i].BlockS);
	}
	//yellow bricks
	for (size_t i = 0; i < 10; i++) {
		window.draw(Yellow1[i].BlockS);
	}
	for (size_t i = 0; i < 10; i++) {
		window.draw(Yellow2[i].BlockS);
	}
	//red bricks
	for (size_t i = 0; i < 10; i++) {
		window.draw(Red[i].BlockS);
	}
	// blue bricks
	for (size_t i = 0; i < 10; i++) {
		window.draw(Blue[i].BlockS);
	}
}

void save() {
	ofstream file("material/scoreboard.txt", ios::app);
	file << User[Userscnt].name << " " << User[Userscnt].score << endl;
	file.close();
}

int input() {
	ifstream otherfile("material/scoreboard.txt");
	int i = 0;
	while (otherfile.good()) {
		otherfile >> User[i].name >> User[i].score;
		i++;
	}
	otherfile.close();
	return i;
}

void EndGame_Menu() {
	EndGameMenuTexture.loadFromFile("material/ENDGAME MENU.jpg");
	EndGameMenu.setTexture(EndGameMenuTexture);

	Event Click;
	while (window.pollEvent(Click)) {
		if (Mouse::isButtonPressed(Mouse::Left)) {
			Mouse_Postion = Mouse::getPosition(window);
			if (Mouse_Postion.x > 309 && Mouse_Postion.x < 393 && Mouse_Postion.y > 174 && Mouse_Postion.y < 197) {
				if (confirmation == 0)
					ButtonClick.play();
				User[Userscnt].name = User[Userscnt - 1].name;
				options = 1;
			}
			if (Mouse_Postion.x > 304 && Mouse_Postion.x < 395 && Mouse_Postion.y > 273 && Mouse_Postion.y < 304) {
				if (confirmation == 0)
					ButtonClick.play();
				options = 0;
			}
		}
	}
}

void Name(Event event) {
	entername.setFont(font);

	if (event.type == Event::TextEntered) {
		if (confirmation == 0)
			ButtonClick.play();
		if (Keyboard::isKeyPressed(Keyboard::Key::Enter)) {
			User[Userscnt].name = Playerinput;
			music.stop();
			options = 1;
		}
		if (event.text.unicode < 128 && !Keyboard::isKeyPressed(Keyboard::Key::BackSpace) &&
			!Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
			if (Playerinput.size() < 18) {
				Playerinput += event.text.unicode;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSpace) && !Playerinput.empty()) {
			Playerinput.pop_back();
		}
	}
	PlayerName.setString(Playerinput);
	PlayerName.setCharacterSize(30);
	PlayerName.setPosition(195, 195);
	PlayerName.setFillColor(Color(10, 150, 50));
	cout << Playerinput << endl;

	entername.setPosition(270, 160);
	entername.setCharacterSize(25);
	entername.setString("Enter Your Name :");
	entername.setFillColor(Color(10, 150, 50));
}