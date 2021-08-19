#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const std::vector<VertexBufferElements>& elements = layout.GetElements();
	uint64_t offset = 0;

	for(uint32_t i = 0; i < elements.size(); i++)
	{
		const VertexBufferElements& element = elements[i];

		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset);
		glEnableVertexAttribArray(i);

		offset += element.count * VertexBufferElements::GetSizeOfType(element.type);
	}

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	//glEnableVertexAttribArray(0);
}

void VertexArray::Bind() const 
{ 
	glBindVertexArray(m_RendererID); 
}

void VertexArray::Unbind() const 
{ 
	glBindVertexArray(0); 
}
