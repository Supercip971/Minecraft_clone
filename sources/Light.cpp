#include "Light.h"

// Cr�e une lumi�re

Light::Light()
{
	type = Type::Ambient;
	vector = glm::vec3(1.f, 0.f, 0.f);
	color = Color(0.f, 0.f, 0.f, 0.f);
	intensity = 0.f;
}

// Cr�e une lumi�re � partir d'une autre

Light::Light(const Light& other)
{
	*this = other;
}

// Cr�e une lumi�re � partir de ces propri�t�s

Light::Light(Type type, const glm::vec3& vector, const Color& color, float intensity)
{
	set_type(type);
	set_vector(vector);
	set_color(color);
	set_intensity(intensity);
}

// Op�rateur �gal

Light& Light::operator=(const Light& other)
{
	type = other.type;
	vector = other.vector;
	color = other.color;
	intensity = other.intensity;

	return *this;
}

// Donne le type de la lumi�re

Light::Type Light::get_type() const
{
	return type;
}

// Change le type de la lumi�re

void Light::set_type(Type type)
{
	this->type = type;
}

// Donne la position ou la direction de la lumi�re

glm::vec3 Light::get_vector() const
{
	return vector;
}

// Change la position ou la direction de la lumi�re

void Light::set_vector(const glm::vec3& vector)
{
	if (type == Type::Directional)
	{
		if (vector == glm::vec3(0.f, 0.f, 0.f))
			this->vector = glm::vec3(1.f, 0.f, 0.f);
		
		else
			this->vector = glm::normalize(vector);
	}

	else
		this->vector = vector;
}

// Donne la couleur ambiante

Color Light::get_color() const
{
	return color;
}

// Change la couleur ambiante

void Light::set_color(const Color& color)
{
	this->color = color;
	this->color.r *= this->color.a;
	this->color.g *= this->color.a;
	this->color.b *= this->color.a;
	this->color.a = 1.f;
}

// Donne l'intensit�

float Light::get_intensity() const
{
	return intensity;
}

// Change l'intensit�

void Light::set_intensity(float intensity)
{
	this->intensity = std::max(intensity, 0.f);
}