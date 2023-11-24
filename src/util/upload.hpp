#pragma once

#include <pch.h>
#include <util/file_manager.hpp>

namespace gaboot
{
	using namespace drogon;

	class upload_file
	{
	public:
		explicit upload_file(const Multer* file, std::string filename, std::string folderName) : m_file(file)
		{
			if (!file)
				throw std::runtime_error("File doesn't exist");

			auto folder = g_file_manager.get_project_folder(fmt::format("./{}", folderName));
			m_raw_image = folder.get_file(fmt::format("./pictures/{}.{}", filename, m_file->getFileExtension()));
			m_raw_thumbnail = folder.get_file(fmt::format("./pictures/thumbnail/{}.{}", filename, m_file->getFileExtension())).get_path();

			m_file_image = folder.get_file(fmt::format("./pictures/thumbnail/{}", filename)).relative_path();
			m_file_thumbnail = folder.get_file(fmt::format("./pictures/thumbnail/{}", filename)).relative_path();

			m_image = m_raw_image.relative_path();
			m_thumbnail = m_raw_thumbnail.relative_path();
		}
		virtual ~upload_file() noexcept = default;

		void save() const
		{
			if (this->save_image())
			{
				this->resize_thumbnail();
			}
		}

		bool resize_thumbnail() const
		{
			if (m_file->getFileExtension() == "gif")
			{
				LOG(WARNING) << "GIF file doesn't support resizing";

				return false;
			}
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

		bool convert_to_jpg() const
		{
			// Read the PNG image
			if (m_file->getFileExtension() == "png")
			{
				cv::Mat img = cv::imread(fmt::format("{}.{}", m_file_image.string(), m_file->getFileExtension()));
				cv::Mat thumb = cv::imread(fmt::format("{}.{}", m_file_thumbnail.string(), m_file->getFileExtension()));

				if (img.empty())
				{
					LOG(WARNING) << "Error: Unable to read the input image.";

					return false;
				}

				// Save the image as a JPG file
				cv::imwrite(fmt::format("{}.jpg", m_file_image.string()), img);
				cv::imwrite(fmt::format("{}.jpg", m_file_thumbnail.string()), thumb);

				return true;
			}

			return true;
		}

		bool resize(int width, int height) const
		{
			cv::Mat image = cv::imread(m_thumbnail.string());

			if (image.empty())
			{
				LOG(WARNING) << "Error: Could not open or find the image.";
				return false;
			}

			// Define the new size for the resized image
			cv::Size newSize(width, height);

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
		bool save_image() const
		{
			try
			{
				m_file->saveAs(m_raw_thumbnail.absolute_path().string());
				m_file->saveAs(m_raw_image.absolute_path().string());

				LOG(INFO) << "Thumbnail saved at " << m_raw_thumbnail.absolute_path().string();
				LOG(INFO) << "Image saved at " << m_raw_image.absolute_path().string();

				return true;
			}
			catch (const std::exception& e)
			{
				LOG(WARNING) << e.what();

				return false;
			}
		}
	private:
		const Multer* m_file;
		std::filesystem::path m_image;
		std::filesystem::path m_thumbnail;
		std::filesystem::path m_file_image;
		std::filesystem::path m_file_thumbnail;
		file m_raw_image;
		file m_raw_thumbnail;
	};
}