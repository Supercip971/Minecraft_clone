#ifndef SKIN_H
#define SKIN_H

#include "utils.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "Mesh.h"
#include "VertexBuffer.h"

class Player;

// Classe d�finissant le model 3D du joueur

class Skin
{
public:

	static constexpr float max_walk_strength	= 0.7f;						// Taille maximale des pas
	static constexpr float strength_step		= max_walk_strength / 10.f;	// Vitesse pour atteindre la taille max
	static constexpr float walk_anim_speed		= 0.2f;						// Vitesse des pas
	static constexpr float turn_speed			= 0.1f;						// Vitesse de l'animation de rotation
	static constexpr float max_head_rotation	= pi / 4.f;					// Limite de la rotation de la t�te
	static constexpr float texture_size			= 64.f;						// Taille de la texture du skin

	// Classe d�finissant une partie du model

	class Part
	{
	private:
	
		VertexBuffer				object;				// Vertex buffer OpenGL
		glm::mat4					propagated_matrix;	// Matrice de propagation
		glm::mat4					local_matrix;		// Matrice locale
		std::vector<Part*>			children;			// Parties reli�es au

	public:

									Part();
									Part(const Part& other);
									Part(Mesh mesh, const glm::vec2& texture_pos, const glm::vec3& size, const std::vector<Part*>& children);

		Part&						operator=(const Part& other);

		void						draw(const Camera& camera, const std::vector<const Light*>& lights, const Plane& clipping_plane = Plane(0.f, 1.f, 0.f, 10000.f)) const;

		static std::vector<float>	init_texcoords(const glm::vec2& texture_pos, const glm::vec3& size);

		friend						Skin;
		friend						Player;
	};

private:

	Part		arm_1;				// Bras gauche
	Part		shoulder_1;			// Epaule gauche
	Part		arm_2;				// Bras droit
	Part		shoulder_2;			// Epaule droite
	Part		leg_1;				// Jambe gauche
	Part		hips_1;				// Hanche gauche
	Part		leg_2;				// Jambe droite
	Part		hips_2;				// Hanche droite
	Part		camera;				// Cam�ra
	Part		head;				// T�te
	Part		neck;				// Cou
	Part		body;				// Corps
	bool		walking;			// Est en train de courir
	bool		running;			// Est en train de courir
	bool		forward;			// Est tourn� vers l'avant
	bool		left;				// Est tourn� vers la gauche
	bool		right;				// Est tourn� vers la droite
	bool		back;				// Est tourn� vers l'arri�re
	uint32_t	animation_i;		// Etat actuel de l'animation
	float		walk_strength;		// Taille des pas
	glm::mat4	shoulder_1_initial;	// Matrice initiale de l'�paule gauche
	glm::mat4	shoulder_2_initial;	// Matrice initiale de l'�paule droite
	glm::mat4	hips_1_initial;		// Matrice initiale de la hanche gauche
	glm::mat4	hips_2_initial;		// Matrice initiale de la hanche droite
	glm::mat4	neck_initial;		// Matrice initiale du cou
	float		body_angle;			// Angle du corps
	float		head_angle;			// Angle de la t�te

public:

				Skin();
				Skin(const Skin& other);
				Skin(const glm::vec3& position, const glm::vec3& direction);

	Skin&		operator=(const Skin& other);

	void		move(const glm::vec3& position);
	void		rotate(float angle);
	void		rotate_head(float yaw, float pitch);
	void		update_walk();
	void		draw(const Camera& camera, const std::vector<const Light*>& lights, const Plane& clipping_plane = Plane(0.f, 1.f, 0.f, 10000.f)) const;

	friend		Player;
};

#endif