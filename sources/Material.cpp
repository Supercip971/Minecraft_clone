#include "Material.h"

Material Material::block	= Material(Color(), 0.2f, 0.6f, 0.f, 1.f);
Material Material::entity	= Material(Color(), 0.2f, 0.6f, 0.4f, 100.f);
Material Material::water	= Material(Color(0.f, 0.2f, 0.8f, 1.f), 0.4f, 0.4f, 0.4f, 500.f);

// Cr�e un mat�riaux

Material::Material()
{
	color = Color(0.f, 0.f, 0.f, 0.f);
	ambient = 0.f;
	diffuse = 0.f;
	specular = 0.f;
	shininess = 0.f;
}

// Cr�e un mat�riaux � partir d'un autre

Material::Material(const Material& other)
{
	*this = other;
}

// Cr�e un mat�riaux � partir de ces propri�t�s

Material::Material(const Color& color, float ambient, float diffuse, float specular, float shininess)
{
	this->color = color;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}

// Op�rateur �gal

Material& Material::operator=(const Material& other)
{
	color = other.color;
	ambient = other.ambient;
	diffuse = other.diffuse;
	specular = other.specular;
	shininess = other.shininess;

	return *this;
}

// Donne la couleur

Color Material::get_color() const
{
	return color;
}

// Change la couleur

void Material::set_color(const Color& color)
{
	this->color = color;
}

// Donne le niveau de lumi�re ambiente

float Material::get_ambient() const
{
	return ambient;
}

// Change le niveau de lumi�re ambiente

void Material::set_ambient(float ambient)
{
	this->ambient = normalize(ambient);
}

// Donne le niveau de lumi�re diffuse

float Material::get_diffuse() const
{
	return diffuse;
}

// Change le niveau de lumi�re diffuse

void Material::set_diffuse(float diffuse)
{
	this->diffuse = normalize(diffuse);
}

// Donne le niveau de lumi�re sp�culaire

float Material::get_specular() const
{
	return specular;
}

// Change le niveau de lumi�re sp�culaire

void Material::set_specular(float specular)
{
	this->specular = normalize(specular);
}

// Donne la taille du reflet

float Material::get_shininess() const
{
	return shininess;
}

// Change la taille du reflet

void Material::set_shininess(float shininess)
{
	this->shininess = std::max(shininess, 0.f);
}
