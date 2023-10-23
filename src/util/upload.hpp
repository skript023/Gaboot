#pragma once

#include <pch.h>
#include <util/file_manager.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize2.h"

namespace gaboot
{
	namespace Express::Multer
	{
		using File = HttpFile;
	}
	using namespace drogon;

	class upload_file
	{
	public:
		explicit upload_file(Express::Multer::File const& file, std::string filename, std::string folderName) : m_file(file)
		{
			auto folder = g_file_manager.get_project_folder(fmt::format("./{}", folderName));
			m_raw_image = folder.get_file(fmt::format("./pictures/{}.{}", filename, m_file.getFileExtension()));
			m_raw_thumbnail = folder.get_file(fmt::format("./pictures/thumbnail/{}.{}", filename, m_file.getFileExtension())).get_path();

			m_image = m_raw_image.get_path().lexically_normal();
			m_thumbnail = m_raw_thumbnail.get_path().lexically_normal();
		}
		virtual ~upload_file() noexcept = default;

		void save() const
		{
			m_file.saveAs(m_raw_image.absolute_path().string());
			m_file.saveAs(m_raw_thumbnail.absolute_path().string());
			this->resize();
		}

		bool resize() const
		{
			int inputWidth, inputHeight, inputChannels;
			unsigned char* inputImage = stbi_load(m_thumbnail.string().c_str(), &inputWidth, &inputHeight, &inputChannels, 3);

			LOG(INFO) << "Attempting to load " << m_thumbnail.string().c_str();

			int width = inputWidth * 0.4f;
			int height = inputHeight * 0.4f;

			if (stbi_failure_reason())
			{
				LOG(WARNING) << "Error: " << stbi_failure_reason() << " while load " << this->get_thumbnail_filename();
				
				return false;
			}


			unsigned char* resizedImage = new unsigned char[width * height * inputChannels];

			auto result = stbir_resize_uint8_linear(inputImage, inputWidth, inputHeight, 0, resizedImage, width, height, 0, (stbir_pixel_layout)inputChannels);

			if (stbi_failure_reason())
			{
				LOG(WARNING) << "Error: " << stbi_failure_reason() << " while resize " << this->get_thumbnail_filename();

				stbi_image_free(inputImage);
				delete[] resizedImage;

				return false;
			}

			if (!stbi_write_jpg(m_raw_thumbnail.absolute_path().string().c_str(), width, height, inputChannels, resizedImage, 100))
			{
				LOG(WARNING) << "Error: Failed to save the resized image.";

				return false;
			}

			// Clean up memory
			stbi_image_free(inputImage);
			delete[] resizedImage;

			return true;
		}

		[[nodiscard]] std::string get_image_filename() const { return m_image.has_filename() ? m_image.filename().string() : ""; }
		[[nodiscard]] std::string get_thumbnail_filename() const { return m_thumbnail.has_filename() ? m_thumbnail.filename().string() : ""; }
		[[nodiscard]] std::string get_image_path() const { return m_image.string(); }
		[[nodiscard]] std::string get_thumbnail_path() const { return m_thumbnail.string(); }
	private:
		HttpFile m_file;
		std::filesystem::path m_image;
		std::filesystem::path m_thumbnail;
		file m_raw_image;
		file m_raw_thumbnail;
	};
}