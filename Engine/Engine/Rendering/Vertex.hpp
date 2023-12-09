#pragma once

struct Vertex
{
	Vec4f myPosition;
	Color myColor;
	Vec4f myNormal;
	Vec4f myTangents;
	Vec4f myBinormals;
	Vec2f myTexCoords[4];

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
			descs.Add({ 0, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(Vertex, myPosition) });
			descs.Add({ 1, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(Vertex, myColor) });
			descs.Add({ 2, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(Vertex, myNormal) });
			descs.Add({ 3, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(Vertex, myTangents) });
			descs.Add({ 4, 0, vk::Format::eR32G32B32A32Sfloat, offsetof(Vertex, myBinormals) });

			descs.Add({ 5, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, myTexCoords) + sizeof(Vec2f) * 0 });
			descs.Add({ 6, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, myTexCoords) + sizeof(Vec2f) * 1 });
			descs.Add({ 7, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, myTexCoords) + sizeof(Vec2f) * 2 });
			descs.Add({ 8, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, myTexCoords) + sizeof(Vec2f) * 3 });
		);
		return descs;
	}
};