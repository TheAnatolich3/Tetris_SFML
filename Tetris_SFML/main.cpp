#include <time.h>
#include <SFML/Graphics.hpp>

using namespace sf;

const int M = 20; // ������ �������� ����
const int N = 10; // ������ �������� ����

int field[M][N] = { 0 }; // ������� ����

// ������ �������-���������
int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // S
	3,5,4,6, // Z
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

struct Point
{
	int x, y;
} a[4], b[4];

// �������� �� ����� �� ������� �������� ����
bool check()
{
	for (int i = 0; i < 4; i++) {
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) 
			return 0;
		else if (field[a[i].y][a[i].x]) 
			return 0;
	}

	return 1;
}

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	RenderWindow window(VideoMode(320, 480), "The Game!");

	// �������� � �������� ��������
	Texture texture, texture_background, texture_frame;
	texture.loadFromFile("Images\\tiles.png");
	texture_background.loadFromFile("Images\\background.png");

	// �������� �������
	Sprite sprite(texture), sprite_background(texture_background);

	sprite.setTextureRect(IntRect(0, 0, 18, 18));

	// ���������� ��� ��������������� ����������� ���������
	int dx = 0;
	bool rotate = false; // ���������� ��� �������� ���������
	int colorNum = 1; // ����
	bool beginGame = true;
	int n = rand() % 7;

	// ���������� ��� ������� � ��������
	float timer = 0.0f, delay = 0.3f;

	// ���� (������)
	Clock clock;

	// ������� ���� ����������: �����������, ���� ������� ����
	while (window.isOpen())
	{
		// �������� �����, ��������� � ������ �������, � ������������ ��� � �������
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		// ������������ ������� � �����
		Event event;
		while (window.pollEvent(event))
		{
			// ������������ ����� �� �������� � ����� ������� ����?
			if (event.type == Event::Closed)
				// ����� ��������� ���
				window.close();

			if (event.type == Event::KeyPressed)
				// ��� ������ � ������� �����?
				if (event.key.code == Keyboard::Up) 
					rotate = true;
				// ��� ����� ������� �����?
				else if (event.key.code == Keyboard::Left) 
					dx = -1;
				// ��� ������� ������?
				else if (event.key.code == Keyboard::Right) 
					dx = 1;
		}

		// ������ �� ������ "����"? �������� ������� ���������
		if (Keyboard::isKeyPressed(Keyboard::Down)) 
			delay = 0.05f;

		// �������������� �����������
		for (int i = 0; i < 4; i++) {
			b[i] = a[i];
			a[i].x += dx;
		}

		// ���� ����� �� ������� ���� ����� �����������, �� ���������� ������ ����������

		if (!check()) {
			for (int i = 0; i < 4; i++)
				a[i] = b[i];
		}
		// ��������
		if (rotate)
		{
			Point p = a[1]; // ��������� ����� ��������
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y; // y - y0
				int y = a[i].x - p.x; // x - x0
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
		}

		// ���� ����� �� ������� ���� ����� ��������, �� ���������� ������ ���������� 
		if (!check()) {
			for (int i = 0; i < 4; i++)
				a[i] = b[i];
		}

		// �������� ��������� ���� (���� �������)
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++) {
				b[i] = a[i];
				a[i].y += 1;
			}

			if (!check())
			{
				for (int i = 0; i < 4; i++) 
					field[b[i].y][b[i].x] = colorNum;
				colorNum = 1 + rand() % 7;
				n = rand() % 7;
				for (int i = 0; i < 4; i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}

			}

			timer = 0;
		}

		if (beginGame)
		{
			beginGame = false;
			n = rand() % 7;
			for (int i = 0; i < 4; i++)
			{
				a[i].x = figures[n][i] % 2;
				a[i].y = figures[n][i] / 2;
			}
		}
		dx = 0;
		rotate = false;
		delay = 0.3f;

		//----�������� �����----//
		int k = M - 1;
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < N; j++)
			{
				if (field[i][j]) count++;
				field[k][j] = field[i][j];
			}
			if (count < N) k--;
		}

		//----���������----//
		
		// ��������� ����� ���� - �����
		window.clear(Color::White);
		window.draw(sprite_background);

		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				if (field[i][j] == 0)
					continue;
				sprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
				sprite.setPosition(j * 18.0f, i * 18.0f);
				sprite.move(28, 31);
				window.draw(sprite);
			}
		}

		for (int i = 0; i < 4; i++)
		{
			// ������������� ���������
			sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));

			// ������������� ������� ������� ������� ���������
			sprite.setPosition(a[i].x * 18.0f, a[i].y * 18.0f);
			sprite.move(28, 31); // ��������
			// ��������� �������
			window.draw(sprite);
		}

		// ��������� ����
		window.display();
	}

	return 0;
}