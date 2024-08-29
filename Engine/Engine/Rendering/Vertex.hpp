#pragma once

struct Vertex
{
	glm::vec3 myPosition; // x, y, z
	int32_t myColor;
	glm::vec3 myNormal; // x, y, z 
	glm::vec3 myTangents; // x, y, z
	glm::vec3 myBinormals; // x, y, z
	glm::vec2 myTexCoords[2];
	
	void PackColor(const glm::vec4& inColor)
	{
		uint8_t r = static_cast<uint8_t>(inColor.r * 255.0f);
		uint8_t g = static_cast<uint8_t>(inColor.g * 255.0f);
		uint8_t b = static_cast<uint8_t>(inColor.b * 255.0f);
		uint8_t a = static_cast<uint8_t>(inColor.a * 255.0f);
		myColor = (a << 24) | (b << 16) || (g << 8) | r;
	}
	
	static List<vk::VertexInputBindingDescription>& GetBindingDescriptions()
	{
		static List<vk::VertexInputBindingDescription> bindingDescriptions;

		DO_ONCE(
			bindingDescriptions.Add(vk::VertexInputBindingDescription()
				.setBinding(0)
				.setStride(sizeof(Vertex))
				.setInputRate(vk::VertexInputRate::eVertex)
			);
		);

		return bindingDescriptions;
	}

	static List<vk::VertexInputAttributeDescription>& GetAttributeDescriptions()
	{
		static List<vk::VertexInputAttributeDescription> descs{};

		DO_ONCE(
			descs.Add({ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, myPosition) });
			descs.Add({ 1, 0, vk::Format::eR32Sint, offsetof(Vertex, myColor) });
			descs.Add({ 2, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, myNormal) });
			descs.Add({ 3, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, myTangents) });
			descs.Add({ 4, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, myBinormals) });

			descs.Add({ 5, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, myTexCoords) + sizeof(glm::vec2) * 0 });
			descs.Add({ 6, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, myTexCoords) + sizeof(glm::vec2) * 1 });
		);
		return descs;
	}
};