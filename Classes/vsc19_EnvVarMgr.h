
#pragma  once

#include <string>
#include <vector>
#include <map>

namespace emcil 
{
	/** This is a simple wrapper around putenv() and getenv() proving 
	  * persistent storage for the values passed to putenv() (in the form of �name=value�) 
  */
	class EnvVariablesManager 
	{
	public:
		using VariableContainer = std::vector<char>;
		using Variables = std::map<std::string, VariableContainer>;

		EnvVariablesManager() {}

		// This class is not copyable.
		EnvVariablesManager(const EnvVariablesManager&)=delete;
		EnvVariablesManager& operator=(const EnvVariablesManager&)=delete;

		void put( const std::string& name, const std::string& value);
		std::string get( const std::string& name) const;
		void del( const std::string& name);

		static void PutOSVariable( char* value);
		static std::string GetOSVariable( const char* name);
		static bool IsOSVariableSet( const char* name);

	private:
		VariableContainer PairToContainer( const std::string& name,
           const std::string& value) const;
		Variables m_vars;
	};
}