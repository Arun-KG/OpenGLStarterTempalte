#include <iostream>
#include <cassert>

#include "IndexBuffer.h"

#include "glad/glad.h"

IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count)
	:m_Count(count)
{
	// Check for size of uint32_t == GLuint
	assert(sizeof(uint32_t) == sizeof(GLuint));

	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, data, GL_STATIC_DRAW);

}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
