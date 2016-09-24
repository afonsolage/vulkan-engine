#pragma once


class Logging
{
public:
	static void init();

	static void info(const char* format, ...);
	static void warn(const char* format, ...);
	static void error(const char* format, ...);
	static void fatal(const char* format, ...);
	static void debug(const char* format, ...);
	static void trace(const char* format, ...);

#ifdef WIN32
	static const int m_severity_info_color;
	static const int m_severity_warning_color;
	static const int m_severity_error_color;
	static const int m_severity_fatal_color;
	static const int m_severity_debug_color;
	static const int m_severity_trace_color;
	static const int m_severity_default_color;
#else
	static const char* m_severity_info_color;
	static const char* m_severity_warning_color;
	static const char* m_severity_error_color;
	static const char* m_severity_fatal_color;
	static const char* m_severity_debug_color;
	static const char* m_severity_trace_color;
	static const char* m_severity_default_color;
#endif

private:
	static const size_t m_buffer_length;
};
