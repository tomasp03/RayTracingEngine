#include "Renderer.h"

static glm::vec3 LerpVec3(glm::vec3 a, glm::vec3 b, float t)
{
    return glm::vec3(std::lerp(a.x, b.x, t), std::lerp(a.y, b.y, t), std::lerp(a.z, b.z, t));
}

static uint32_t ConvertToRGBA(glm::vec4 color)
{
    uint8_t r = (uint8_t)(color.r * 255.0f);
    uint8_t g = (uint8_t)(color.g * 255.0f);
    uint8_t b = (uint8_t)(color.b * 255.0f);
    uint8_t a = (uint8_t)(color.a * 255.0f);

    uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
    return result;
}

static glm::vec4 GammaCorection2(glm::vec4 color)
{
    return glm::vec4(glm::sqrt(color));
}

Renderer::Renderer(int width, int height)
{
    random.Init();
    m_image.size = glm::ivec2(width, height);
    m_accumulationImage = new glm::vec4[m_image.size.x * m_image.size.y];
    m_image.data = new uint32_t[m_image.size.x * m_image.size.y];



    ImageVerticalIter.resize(m_image.size.y);
    for (int y = 0; y < m_image.size.y; y++)
    {
        ImageVerticalIter[y] = y;
    }
}

void Renderer::Resize(glm::ivec2 size)
{
    if (size == m_image.size)
        return;

    m_image.size = size;
    ImageVerticalIter.resize(m_image.size.y);
    for (int y = 0; y < m_image.size.y; y++)
    {
        ImageVerticalIter[y] = y;
    }

    delete[] m_accumulationImage;
    delete[] m_image.data;
    m_accumulationImage = new glm::vec4[m_image.size.x * m_image.size.y];
    m_image.data = new uint32_t[m_image.size.x * m_image.size.y];
}

Image* Renderer::Render(Scene& scene, Camera& camera)
{
    m_ActiveScene = &scene;
    m_ActiveCamera = &camera;

    if (frameIndex == 1)
        memset(m_accumulationImage, 0, m_image.size.x * m_image.size.y * sizeof(glm::vec4));

    std::for_each(std::execution::par, ImageVerticalIter.begin(), ImageVerticalIter.end(),
        [&](unsigned int y)
        {
            for (int x = 0; x < m_image.size.x; x++)
            {
                int index = (x + y * m_image.size.x);
                float offset_u = random.Float2(-0.3f, 0.3f) / (float)m_image.size.x;
                float offset_v = random.Float2(-0.3f, 0.3f) / (float)m_image.size.y;

                float u = ((float)(x) / (float)m_image.size.x) + offset_u;
                float v = ((float)(y) / (float)m_image.size.y) + offset_v;


                Ray ray = m_ActiveCamera->get_ray(u, v);

                m_accumulationImage[index] += Trace(ray);

                glm::vec4 accumulationColor = m_accumulationImage[index];
                accumulationColor /= (float)frameIndex;
                accumulationColor.a = 1.0f;
                m_image.data[index] = ConvertToRGBA(GammaCorection2(accumulationColor));
            }
        }
    );

    if (m_Settings.Accumulate && !m_ActiveCamera->IsMoving())
        frameIndex++;
    else
        frameIndex = 1;

    return &m_image;
}

HitInfo Renderer::CalculateRayCollision(Ray ray)
{
    HitInfo closestHit;
    closestHit.didHit = false;
    closestHit.dst = FLT_MAX;

    HitInfo closestHitPlane;
    closestHitPlane.didHit = false;
    closestHitPlane.dst = FLT_MAX;

    const std::vector<Shape*>& shapes = m_ActiveScene->shapes;

    for (int i = 0; i < shapes.size(); i++)
    {
        Shape* shape = shapes[i];
        HitInfo hitInfo = shape->Intersection(ray);

        if (hitInfo.didHit && hitInfo.dst < closestHit.dst)
        {
            closestHit = hitInfo;
            closestHit.mat = shape->material();
        }
    }

    if (!closestHit.didHit && !closestHitPlane.didHit)
        return closestHit;

    return closestHit.dst < closestHitPlane.dst ? closestHit : closestHitPlane;
}


glm::vec4 Renderer::BackgroundColor(Ray ray)
{
    glm::vec3 unit_direction = glm::normalize(ray.direction);
    float t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * glm::vec4(1.0, 1.0, 1.0, 1.0) + t * glm::vec4(0.5, 0.7, 1.0, 1.0);
}

glm::vec4 Renderer::Trace(Ray& ray)
{
    glm::vec4 incomingLight = glm::vec4(0.0, 0.0, 0.0, 0.0);
    glm::vec4 rayColor = glm::vec4(1, 1, 1, 1);
    HitInfo firstHitInfo = CalculateRayCollision(ray);

    for (int i = 0; i <= MaxBounces; i++)
    {
        HitInfo hitInfo = CalculateRayCollision(ray);
        if (hitInfo.didHit)
        {

            Material material = hitInfo.mat;
            ray.origin = hitInfo.hitPoint;
            glm::vec3 diffuseDir = glm::normalize(hitInfo.normal + random.InUnitSphere());
            glm::vec3 specularDir = glm::reflect(ray.direction, hitInfo.normal);
            bool isSpecularBounce = material.specularProbability >= random.Float();
            ray.direction = LerpVec3(diffuseDir, specularDir, material.smoothness * isSpecularBounce);

            glm::vec4 emittedLight = material.emissionColor * material.emissionStrength;
            incomingLight += (emittedLight * rayColor);
            rayColor *= glm::vec4(LerpVec3(material.albido, material.specularColor, isSpecularBounce), 1.0f);
        }
        else
        {
            incomingLight += Skybox(ray) * rayColor;
            break;
        }
    }

    return glm::clamp(incomingLight, glm::vec4(0.0f), glm::vec4(1.0f));
}

glm::vec4 Renderer::Skybox(Ray ray)
{
    glm::vec3 dir = glm::normalize(ray.direction);
    float u = 0.5f + (glm::atan(dir.x, dir.z)) / (2.0f * PI);
    float v = 0.5f + (glm::asin(dir.y)) / PI;

    int x = (int)(u * (m_ActiveScene->skybox.x - 1));
    int y = (int)(v * (m_ActiveScene->skybox.y - 1));

    int index = (x + y * m_ActiveScene->skybox.x) * m_ActiveScene->skybox.chanels;

    glm::vec4 color = { m_ActiveScene->skybox.image[index + 0], m_ActiveScene->skybox.image[index + 1], m_ActiveScene->skybox.image[index + 2], 1.0f };

    return color;
}
