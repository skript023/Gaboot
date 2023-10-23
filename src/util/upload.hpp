#pragma once

#include <pch.h>
#include <util/file_manager.hpp>

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
			auto image = folder.get_file(fmt::format("./pictures/{}.{}", filename, m_file.getFileExtension())).get_path();
			auto thumbnail = folder.get_file(fmt::format("./pictures/thumbnail/{}.{}", filename, m_file.getFileExtension())).get_path();

			m_image = image.lexically_normal();
			m_thumbnail = thumbnail.lexically_normal();
		}
		virtual ~upload_file() noexcept = default;

		void save() const
		{
			m_file.saveAs(m_image.string());
			m_file.saveAs(m_thumbnail.string());
		}

		[[nodiscard]] std::string get_image_filename() { return m_image.has_filename() ? m_image.filename().string() : ""; }
		[[nodiscard]] std::string get_thumbnail_filename() { return m_thumbnail.has_filename() ? m_thumbnail.filename().string() : ""; }
		[[nodiscard]] std::string get_image_path() { return m_image.string(); }
		[[nodiscard]] std::string get_thumbnail_path() { return m_thumbnail.string(); }
	private:
		HttpFile m_file;
		std::filesystem::path m_image;
		std::filesystem::path m_thumbnail;
	};
}