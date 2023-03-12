#pragma once

#include "EnginePch.h"

struct Vertex
{
	Vec4f myPosition;
	Color myColor;

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
		);
		return descs;
	}
};