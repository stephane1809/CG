#include "Camera.h"
#include <SDL2/SDL.h> // Lib gr�fica
#include <iostream>

/**
 * @brief Construtor da classe Camera.
 *
 * @param position A posição da câmera.
 * @param lookAt O ponto para onde a câmera está olhando.
 * @param viewUp O vetor de orientação da câmera.
 */
Camera::Camera(Eigen::Vector3d position, Eigen::Vector3d lookAt, Eigen::Vector3d viewUp)
	: position(position), lookAt(lookAt), viewUp(viewUp), speed(5.0)
{
	this->position = position;
	this->lookAt = lookAt;
	this->viewUp = viewUp;

	// Calcula os vetores i, j e k.
	this->k = (position - lookAt).normalized();
	this->i = (viewUp.cross(k)).normalized();
	this->j = k.cross(i);

	// matriz de transformação da câmera.
	this->transformationMatrix << i[0], i[1], i[2], -(i.dot(position)),
		j[0], j[1], j[2], -(j.dot(position)),
		k[0], k[1], k[2], -(k.dot(position)),
		0, 0, 0, 1;
}

/**
 * @brief Retorna a matriz de transformação da câmera.
 *
 * @return A matriz de transformação da câmera.
 */
Eigen::Matrix4d Camera::getTransformationMatrix()
{
	return this->transformationMatrix;
}

void Camera::processInput(const Uint8 *keystates, double speed)
{
	std::cout << "Teclas sendo processadas..." << std::endl;
	std::cout << "Antes da movimentação: " << position.transpose() << std::endl;

	// Movimentação para frente e para trás (W e S)
	if (keystates[SDL_SCANCODE_W])
	{
		std::cout << "Movendo para frente" << std::endl;
		position -= k * speed; // Move na direção da câmera
	}
	if (keystates[SDL_SCANCODE_S])
	{
		std::cout << "Movendo para trás" << std::endl;
		position += k * speed; // Move para trás
	}

	// Movimentação lateral (A e D)
	if (keystates[SDL_SCANCODE_A])
	{
		std::cout << "Movendo para esquerda" << std::endl;
		position -= i * speed; // Move para a esquerda
	}
	if (keystates[SDL_SCANCODE_D])
	{
		std::cout << "Movendo para direita" << std::endl;
		position += i * speed; // Move para a direita
	}

	std::cout << "Depois da movimentação: " << position.transpose() << std::endl;
	updateCameraMatrix();
}

void Camera::updateCameraMatrix()
{
	// Recalcula os vetores da câmera após a movimentação
	this->k = (lookAt - position).normalized();
	this->i = viewUp.cross(k).normalized();
	this->j = k.cross(i).normalized();

	// Atualiza a matriz de transformação
	this->transformationMatrix << i[0], i[1], i[2], -(i.dot(position)),
		j[0], j[1], j[2], -(j.dot(position)),
		k[0], k[1], k[2], -(k.dot(position)),
		0, 0, 0, 1;
}