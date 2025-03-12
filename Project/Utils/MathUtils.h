#pragma once
#include <SDL.h>

namespace MathUtils {
    // Вычисление длины вектора
    float Length(const SDL_FPoint& vector);
    // Нормализация вектора
    SDL_FPoint Normalize(const SDL_FPoint& vector);
    // Скалярное умножение вектора на число
    SDL_FPoint Multiply(const SDL_FPoint& vector, float scalar);
    // Скалярное деление вектора на число
    SDL_FPoint Divide(const SDL_FPoint& vector, float scalar);
    // Разность двух векторов
    SDL_FPoint Subtract(const SDL_FPoint& a, const SDL_FPoint& b);
    // Сумма двух векторов
    SDL_FPoint Add(const SDL_FPoint& a, const SDL_FPoint& b);
    // Расчет расстояния между двумя точками
    float Distance(const SDL_FPoint& a, const SDL_FPoint& b);
    // Скалярное произведение двух векторов
    float Dot(const SDL_FPoint& a, const SDL_FPoint& b);
    // // Функция для инверсии вектора
    SDL_FPoint Negate(const SDL_FPoint& vector);
    // Генерация случайного числа от 0 до 1
    float NextFloat();
    // Масштабирование вектора
    SDL_FPoint Scale(const SDL_FPoint& vector, float scale);

    float Clamp(float value, float min, float max);


}