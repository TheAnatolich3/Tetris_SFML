#include <SFML/Graphics.hpp>

using namespace sf;

const int M = 20; // высота игрового поля
const int N = 10; // ширина игрового поля

int field[M][N] = { 0 }; // игровое поле

// Массив фигурок-тетрамино
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

int main()
{
	RenderWindow window(VideoMode(320, 480), "The Game!");

	// Создание и загрузка текстуры
	Texture texture;
	texture.loadFromFile("Images\\tiles.png");

	// Создание спрайта
	Sprite sprite(texture);

	sprite.setTextureRect(IntRect(0, 0, 18, 18));

	// Переменная для горизонтального перемещения тетрамино
	int dx = 0;
	bool rotate = false; // переменная для вращения тетрамино

	// Переменные для таймера и задержки
	float timer = 0, delay = 0.3;

	// Часы (таймер)
	Clock clock;

	// Главный цикл приложения: выполняется, пока открыто окно
	while (window.isOpen())
	{
		// Получаем время, прошедшее с начала отсчета, и конвертируем его в секунды
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		// Обрабатываем события в цикле
		Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == Event::Closed)
				// тогда закрываем его
				window.close();

			if (event.type == Event::KeyPressed)
				// Эта кнопка – стрелка вверх?
				if (event.key.code == Keyboard::Up) 
					rotate = true;
				// Или может стрелка влево?
				else if (event.key.code == Keyboard::Left) 
					dx = -1;
				// Или стрелка вправо?
				else if (event.key.code == Keyboard::Right) 
					dx = 1;
		}

		for (int i = 0; i < 4; i++) 
			a[i].x += dx;

		// Вращение
		if (rotate)
		{
			Point p = a[1]; // указываем центр вращения
			for (int i = 0; i < 4; i++)
			{
				int x = a[i].y - p.y; // y - y0
				int y = a[i].x - p.x; // x - x0
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
		}

		// Движение тетрамино вниз («тик» таймера)
		if (timer > delay)
		{
			for (int i = 0; i < 4; i++) 
				a[i].y += 1;
			timer = 0;
		}

		int n = 3; // задаем тип тетрамино
		// Первое появление тетрамино на поле?
		if (a[0].x == 0)
		{
			for (int i = 0; i < 4; i++)
			{
				a[i].x = figures[n][i] % 2;
				a[i].y = figures[n][i] / 2;
			}
		}
		dx = 0;
		rotate = false;

		// Установка цвета фона - белый
		window.clear(Color::White);


		for (int i = 0; i < 4; i++)
		{
			// Устанавливаем позицию каждого кусочка тетрамино
			sprite.setPosition(a[i].x * 18.0f, a[i].y * 18.0f);
			// Отрисовка спрайта
			window.draw(sprite);
		}

		// Отрисовка окна
		window.display();
	}

	return 0;
}