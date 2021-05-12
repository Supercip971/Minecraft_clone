#include "Game.h"

World					Game::world;
Sun						Game::sun;
Moon					Game::moon;
Sky						Game::sky;
std::stack<glm::mat4>	Game::matrices = {};
Player					Game::player;
Camera					Game::fixed_cam;
bool					Game::fix_cam = false;
bool					Game::stop_moving = false;
float					Game::time = 0.f;

// Initialise le jeu

void Game::init()
{
	// Initialisations
	Shader::init();
	Texture::init();
	FrameBuffer::init();

	// Player
	matrices.push(glm::mat4(1.f));
	player = Player(glm::vec3(0.f, Chunk::height + 3.f, 0.f));

	// World
	world.init(player.get_position());
	sun.init();
	moon.init();
	sky.init();
}

// Prend en compte les �venements

void Game::checks_events(const SDL_Event& my_event)
{
	if (my_event.type == SDL_KEYUP)
	{
		// Voler
		if (my_event.key.keysym.sym == SDLK_m)
			player.fly = !player.fly;

		// Changer de vue
		if (my_event.key.keysym.sym == SDLK_F5)
			player.first_person = !player.first_person;

		// Fixer la cam�ra
		if (my_event.key.keysym.sym == SDLK_o)
		{
			fix_cam = !fix_cam;
			fixed_cam = player.camera;
		}
	}

	// Quitter la fen�tre
	if ((my_event.type == SDL_KEYUP && my_event.key.keysym.sym == SDLK_ESCAPE) || (stop_moving && my_event.type == SDL_MOUSEBUTTONUP))
	{
		stop_moving = !stop_moving;
		SDL_ShowCursor(stop_moving);
		SDL_WarpMouseInWindow(Window::window, Window::center.x, Window::center.y);
		glm::ivec2 mouse_pos = Window::center;
		SDL_Delay(100);
	}
}

// Met � jour le jeu

void Game::update(glm::ivec2& mouse_pos)
{
	time += frame_duration;

	// D�placement
	player.move(SDL_GetKeyboardState(NULL), world);

	// Cam�ra

	if (!stop_moving)
	{
		SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
		player.look(mouse_pos);
		SDL_WarpMouseInWindow(Window::window, Window::center.x, Window::center.y);
	}

	player.update_head();

	// G�n�ration
	world.send_meshes();

	// M�t�o
	sun.update(player.get_position());
	moon.update(player.get_position());
	sky.update(player.get_position(), sun.get_position().y);
}

// Affiche les �l�ments du jeu

void Game::draw()
{
	// Fait le rendu de la reflection

	Camera render_camera = fix_cam ? fixed_cam : player.camera;
	render_camera.invert(water_level);
	render_camera.change_resolution(Window::size.x * reflection_quality, Window::size.y * reflection_quality);

	FrameBuffer::reflection.bind();
	{
		FrameBuffer::clear();

		player.draw(render_camera, { sun.get_light() }, Plane(0.f, 1.f, 0.f, -water_level));
		world.draw(render_camera, { sun.get_light() }, Plane(0.f, 1.f, 0.f, -water_level + 0.4f));
		sun.draw(render_camera);
		moon.draw(render_camera);
		sky.draw(render_camera);
	}
	FrameBuffer::unbind();

	// Fait le rendu de la refraction

	render_camera = fix_cam ? fixed_cam : player.camera;
	render_camera.change_resolution(Window::size.x * refraction_quality, Window::size.y * refraction_quality);

	FrameBuffer::refraction.bind();
	{
		FrameBuffer::clear();

		player.draw(render_camera, { sun.get_light() }, Plane(0.f, -1.f, 0.f, water_level));
		world.draw(render_camera, { sun.get_light() }, Plane(0.f, -1.f, 0.f, water_level + 0.6f));
	}
	FrameBuffer::unbind();

	// Fait le rendu du jeu

	render_camera.change_resolution(Window::size.x, Window::size.y);

	FrameBuffer::game.bind();
	{
		FrameBuffer::clear();

		player.draw(render_camera, { sun.get_light() }, Plane(0.f, 1.f, 0.f, -water_level));
		world.draw(render_camera, { sun.get_light() }, Plane(0.f, 1.f, 0.f, -water_level));
		sun.draw(render_camera);
		moon.draw(render_camera);
		sky.draw(render_camera);

		FrameBuffer::reflection.get_texture().bind(0);
		FrameBuffer::refraction.get_texture().bind(1);
		{
			world.draw_water(render_camera, { sun.get_light() });
		}
		Texture::unbind();
		Texture::unbind();
	}
	FrameBuffer::unbind();

	// Fait le rendu du final

	VertexBuffer game(Shader::screen, glm::scale(glm::mat4(1.f), glm::vec3(2.f, 2.f, 2.f)) * glm::rotate(glm::mat4(1.f), pi, glm::vec3(0.1f, 0.f, 0.f)) * Mesh::square);

	Shader::screen.bind();
	FrameBuffer::game.get_texture().bind(0);
	game.bind();
	{
		game.send_texture("u_texture", 0);
		game.draw();
	}
	VertexBuffer::unbind();
	Texture::unbind();
	Shader::unbind();
}
