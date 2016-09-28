#include "stdafx.h"
#include "Logging.h"
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/value_extraction.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/basic_sink_backend.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

#ifdef WIN32
#include <windows.h>
#endif

namespace logging = boost::log;
namespace sinks = logging::sinks;
namespace expressions = logging::expressions;
namespace keywords = logging::keywords;

using severity_level = logging::trivial::severity_level;


const size_t Logging::m_buffer_length{ 1024 };
#ifdef WIN32

const int Logging::m_severity_info_color{ 15 };
const int Logging::m_severity_warning_color{ 14 };
const int Logging::m_severity_error_color{ 12 };
const int Logging::m_severity_fatal_color{ 207 };
const int Logging::m_severity_debug_color{ 11 };
const int Logging::m_severity_trace_color{ 13 };
const int Logging::m_severity_default_color{ 7 };

#else

const char* Logging::m_severity_info_color{ "\033[37m" };
const char* Logging::m_severity_warning_color{ "\033[93m" };
const char* Logging::m_severity_error_color{ "\033[91m" };
const char* Logging::m_severity_fatal_color{ "\033[48;5;196m" };
const char* Logging::m_severity_debug_color{ "\033[96m" };
const char* Logging::m_severity_trace_color{ "\033[95m" };
const char* Logging::m_severity_default_color{ "\033[0m" };

#endif


struct custom_sink : public sinks::basic_formatted_sink_backend<char, sinks::concurrent_feeding>
{
#ifdef WIN32
	HANDLE h_stdout;
#endif

	void init()
	{
#ifdef WIN32
		h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

		if (h_stdout == INVALID_HANDLE_VALUE)
			throw std::runtime_error("Invalid to get Win32 console handle.");
#endif
	}

	void before_consume(severity_level severity)
	{
		switch (severity)
		{
		case severity_level::info:
		{
#ifdef WIN32
			SetConsoleTextAttribute(h_stdout, Logging::m_severity_info_color);
#else
			std::cout << Logging::m_severity_info_color;
#endif
			break;
		}
		case severity_level::warning:
		{
#ifdef WIN32
			SetConsoleTextAttribute(h_stdout, Logging::m_severity_warning_color);
#else
			std::cout << Logging::m_severity_warning_color;
#endif
			break;
		}
		case severity_level::error:
		{
#ifdef WIN32
			SetConsoleTextAttribute(h_stdout, Logging::m_severity_error_color);
#else
			std::cout << Logging::m_severity_error_color;
#endif
			break;
		}
		case severity_level::fatal:
		{
#ifdef WIN32
			SetConsoleTextAttribute(h_stdout, Logging::m_severity_fatal_color);
#else
			std::cout << Logging::m_severity_fatal_color;
#endif
			break;
		}
		case severity_level::debug:
		{
#ifdef WIN32
			SetConsoleTextAttribute(h_stdout, Logging::m_severity_debug_color);
#else
			std::cout << Logging::m_severity_debug_color;
#endif
			break;
		}
		case severity_level::trace:
		{
#ifdef WIN32
			SetConsoleTextAttribute(h_stdout, Logging::m_severity_trace_color);
#else
			std::cout << Logging::m_severity_trace_color;
#endif
			break;
		}
		}
	}

	void after_consume()
	{
#ifdef WIN32
		SetConsoleTextAttribute(h_stdout, Logging::m_severity_default_color);
#else
		std::cout << Logging::m_severity_default_color;
#endif
	}

	void do_consume(const logging::record_view& record_view, const std::string& message)
	{

		std::cout << message << std::endl;
	}

	void consume(const logging::record_view& record_view, const std::string message)
	{
		auto severity = record_view[logging::trivial::severity];
		if (severity)
		{
			before_consume(severity.get());
		}

		do_consume(record_view, message);

		after_consume();
	}
};

void Logging::init()
{
	using sink_t = sinks::asynchronous_sink<custom_sink>;


	boost::shared_ptr<sink_t> sink = boost::make_shared<sink_t>();
	sink->locked_backend()->init();

	logging::add_common_attributes();
	logging::formatter formatter =
		expressions::stream
		<< "[" << expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S") << "]"
		<< "[" << expressions::attr< severity_level, char >("Severity") << "] "
		<< expressions::message;

	sink->set_formatter(formatter);
	logging::core::get()->add_sink(sink);
}

void Logging::info(const char* format, ...)
{
	char tmp_buffer[m_buffer_length];

	std::va_list args;
	va_start(args, format);
	vsnprintf(tmp_buffer, m_buffer_length, format, args);
	va_end(args);

	BOOST_LOG_TRIVIAL(info) << tmp_buffer;
}

void Logging::warn(const char* format, ...)
{
	char tmp_buffer[m_buffer_length];

	std::va_list args;
	va_start(args, format);
	vsnprintf(tmp_buffer, m_buffer_length, format, args);
	va_end(args);

	BOOST_LOG_TRIVIAL(warning) << tmp_buffer;
}

void Logging::error(const char* format, ...)
{
	char tmp_buffer[m_buffer_length];

	std::va_list args;
	va_start(args, format);
	vsnprintf(tmp_buffer, m_buffer_length, format, args);
	va_end(args);

	BOOST_LOG_TRIVIAL(error) << tmp_buffer;
}

void Logging::fatal(const char* format, ...)
{
	char tmp_buffer[m_buffer_length];

	std::va_list args;
	va_start(args, format);
	vsnprintf(tmp_buffer, m_buffer_length, format, args);
	va_end(args);

	BOOST_LOG_TRIVIAL(fatal) << tmp_buffer;
}

void Logging::debug(const char* format, ...)
{
#ifndef NDEBUG
	char tmp_buffer[m_buffer_length];

	std::va_list args;
	va_start(args, format);
	vsnprintf(tmp_buffer, m_buffer_length, format, args);
	va_end(args);

	BOOST_LOG_TRIVIAL(debug) << tmp_buffer;
#endif
}

void Logging::trace(const char* format, ...)
{
#ifndef NDEBUG
	char tmp_buffer[m_buffer_length];

	std::va_list args;
	va_start(args, format);
	vsnprintf(tmp_buffer, m_buffer_length, format, args);
	va_end(args);

	BOOST_LOG_TRIVIAL(trace) << tmp_buffer;
#endif
}