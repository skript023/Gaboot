#include "category_service.hpp"
#include <util/exception.hpp>
#include <util/upload.hpp>

namespace gaboot
{
    HttpResponsePtr category_service::create(HttpRequestPtr const& req)
    {
		try
		{
			MultiPartParser fileUpload;

			if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() == 0)
			{
				return BadRequestException("Requirement doesn't match").response();
			}

			auto& file = fileUpload.getFiles()[0];

			if (!util::multipart_tojson(fileUpload, m_data)) return BadRequestException("Unknown error").response();

			validator schema({
				{"name", "type:string|required|alphabetOnly"},
				{"description", "type:string|required|alphabetOnly"}
			});

			Categories category(m_data);

			upload_file upload(file, trantor::Date::now().toDbStringLocal(), "categories");

			category.setCreatedat(trantor::Date::now());
			category.setUpdatedat(trantor::Date::now());
			category.setImagepath(upload.get_image_path());
			category.setThumbnailpath(upload.get_thumbnail_path());

			if (!schema.validate(category.toJson(), m_error))
			{
				return BadRequestException(m_error).response();
			}
			
			db().insert(category);

			upload.save();

			m_response.m_message = "Create product success";
			m_response.m_success = true;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

			return response;
		}
		catch (const DrogonDbException& e)
		{
			LOG(WARNING) << e.base().what();

			m_response.m_message = e.base().what();
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(HttpStatusCode::k500InternalServerError);

			return response;
		}
    }
    HttpResponsePtr category_service::findAll(HttpRequestPtr const& req)
    {
		try
		{
			auto& limitParam = req->getParameter("limit");
			auto& pageParam = req->getParameter("page");

			const size_t limit = limitParam.empty() && !util::is_numeric(limitParam) ? 10 : stoull(limitParam);
			const size_t page = pageParam.empty() && !util::is_numeric(pageParam) ? 0 : stoull(pageParam) - 1;

			const auto products = db().orderBy(Categories::Cols::_name).limit(limit).offset(page * limit).findFutureAll().get();

			if (products.empty())
			{
				return NotFoundException("No data retrieved").response();
			}

			Json::Value res(Json::arrayValue);

			for (const auto& user : products)
			{
				res.append(user.toJson());
			}

			const size_t lastPage = (products.size() / (limit + (products.size() % limit))) == 0 ? 0 : 1;

			m_response.m_message = "Success retreive products data";
			m_response.m_success = true;
			m_response.m_data = res;
			m_response.m_last_page = lastPage;

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Unable retrieve products data, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			return response;
		}
    }
	HttpResponsePtr category_service::findOne(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Requirement doesn't match").response();
			}

			const auto user = db().findFutureByPrimaryKey(stoll(id)).get();

			if (!user.getId()) return NotFoundException("Unable retrieve customer detail").response();

			m_response.m_message = "Success retrieve user data";
			m_response.m_success = true;
			m_response.m_data = user.toJson();

			return HttpResponse::newHttpJsonResponse(m_response.to_json());
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Cannot retrieve user data, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
	HttpResponsePtr category_service::update(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			MultiPartParser multipart;

			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Parameters requirement doesn't match").response();
			}

			if (multipart.parse(req) != 0)
			{
				return BadRequestException("Requirement doesn't match").response();
			}

			auto& file = multipart.getFiles()[0];

			m_data["updatedAt"] = trantor::Date::now().toDbStringLocal();

			util::multipart_tojson(multipart, m_data);

			upload_file upload(file, std::to_string(trantor::Date::now().microSecondsSinceEpoch()), "categories");

			if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
			{
				m_data["imagePath"] = upload.get_image_path();
				m_data["thumbnailPath"] = upload.get_thumbnail_path();
			}

			auto args = Criteria(Categories::Cols::_id, CompareOperator::EQ, stoll(id));

			// Loop through JSON members and update corresponding database columns
			for (const auto& [column, request] : this->columnMapping)
			{
				if (m_data.isMember(request))
				{
					auto& jsonValue = m_data[request];

					if (!jsonValue.isNull())
					{
						auto record = db().updateFutureBy(column, args, jsonValue.asString());

						if (record.valid() && record.get())
						{
							m_response.m_data[request + "_updated"] = "success";
						}
						else
						{
							return NotFoundException("Unable to update non-existing record.").response();
						}
					}
				}
			}

			if (!m_response.m_data.empty())
			{
				if (multipart.getFiles().size() > 0 && util::allowed_image(file.getFileExtension().data()))
				{
					LOG_INFO << "File saved.";
					upload.save();
				}

				m_response.m_message = "Success update customer data.";
				m_response.m_success = true;

				auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
				return response;
			}
			else
			{
				return BadRequestException("Unable to update costumer").response();
			}
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Unable to update data, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());

			LOG(WARNING) << e.base().what();

			return response;
		}
	}
	HttpResponsePtr category_service::remove(HttpRequestPtr const& req, std::string&& id)
	{
		try
		{
			if (id.empty() || !util::is_numeric(id))
			{
				return BadRequestException("Parameters requirement doesn't match").response();
			}

			const auto record = db().deleteFutureByPrimaryKey(stoll(id)).get();

			if (record != 0)
			{
				m_response.m_message = fmt::format("Delete user on {} successfully", record);
				m_response.m_success = true;

				auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
				return response;
			}

			return NotFoundException("Unable to delete non-existing data").response();
		}
		catch (const DrogonDbException& e)
		{
			m_response.m_message = fmt::format("Failed delete user, error caught on {}", e.base().what());
			m_response.m_success = false;

			auto response = HttpResponse::newHttpJsonResponse(m_response.to_json());
			response->setStatusCode(k500InternalServerError);

			return response;
		}
	}
}