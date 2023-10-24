#pragma once

#include <pch.h>
#include <util/file_manager.hpp>

namespace gaboot
{
	using namespace drogon;

	class upload_file
	{
	public:
		explicit upload_file(Multer const& file, std::string filename, std::string folderName) : m_file(file)
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
			cv::Mat image = cv::imread(m_thumbnail.string());

			if (image.empty()) 
			{
				LOG(WARNING) << "Error: Could not open or find the image.";
				return false;
			}

			int originalWidth = image.cols * 0.4f;
			int originalHeight = image.rows * 0.4f;

			// Define the new size for the resized image
			cv::Size newSize(originalWidth, originalHeight);

			// Resize the image using the cv::resize function
			cv::Mat resizedImage;
			cv::resize(image, resizedImage, newSize);

			// Save the resized image to a file
			cv::imwrite(m_thumbnail.string(), resizedImage);

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