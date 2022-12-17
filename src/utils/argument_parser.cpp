#include "utils/argument_parser.h"
#include "utils/utils.h"
#include <iostream>

using std::unordered_map;
using std::make_pair;
using std::string;
using std::pair;

void ArgumentParser::add_argument(const string& flag, bool required, int* var, 
                                  const string& description) {
  *var = 0;     
  args_[flag] = ArgumentConfig(flag, INT, required, var, description);
}

void ArgumentParser::add_argument(const string& flag, bool required, bool* var, 
                                  const string& description) {
  *var = false;
  args_[flag] = ArgumentConfig(flag, BOOL, required, var, description);
}

void ArgumentParser::add_argument(const string& flag, bool required, string* var, 
                                  const string& description) {
  *var = "";
  args_[flag] = ArgumentConfig(flag, STRING, required, var, description);
}

void ArgumentParser::add_argument(const string& flag, bool required, double* var, 
                                  const string& description) {
  *var = 0.0;
  args_[flag] = ArgumentConfig(flag, DOUBLE, required, var, description);
}

int ArgumentParser::parse(int arg_count, char* arg_values[]) {  
  // load all of the arguments passed via the command line
  for (int i = 1; i < arg_count; ++i) {
    // skip the executable name (the 0th argument) by starting at 1
    string flag = arg_values[i];
    auto iter = args_.find(flag);

    if (iter == args_.end()) {
      std::cout << BOLDRED << "Unknown flag: " << flag << RESET << std::endl;
      continue;
    } 

    // handle case where flag is provided but no value is given (except bools)
    if (iter->second.data_type_ != BOOL) {
      if (i + 1 == arg_count || args_.find(arg_values[i + 1]) != args_.end()) {
        // avoid segfault if the value to the last argument is not provided
        handleInvalidArgument(flag);
        continue;
      }
    }

    void* to_fill = args_[flag].variable_to_fill_;
    args_[flag].filled_ = true;
    
    // if current argument in iteration is valid, parse and save value
    if (iter->second.data_type_ == INT) {
      try {
        *static_cast<int*>(to_fill) = std::stoi(arg_values[++i]);
      } catch (const std::invalid_argument& e) {
        // could not parse integer argument, so fail to parse
        handleInvalidArgument(flag);
        continue;
      }
    } else if (iter->second.data_type_ == BOOL) {
      *static_cast<bool*>(to_fill) = true;
    } else if (iter->second.data_type_ == DOUBLE) {
      try {
        *static_cast<double*>(to_fill) = std::stod(arg_values[++i]);
      } catch (const std::invalid_argument& e) {
        // could not parse integer argument, so fail to parse
        handleInvalidArgument(flag);
        continue;
      }
    } else if (iter->second.data_type_ == STRING) {
      *static_cast<string*>(to_fill) = arg_values[++i];
    }
  }

  // check if any required arguments have not been provided
  bool all_required_args_provided = true;
  for (pair<string, ArgumentConfig> p : args_) {
    if (p.second.required_ && !p.second.filled_) {
      std::cout << BOLDRED << "Missing required argument " << p.first
                << ": [" << typeAsString(p.second.data_type_) << "] "
                << p.second.description_ << RESET << std::endl;
      all_required_args_provided = false;
    }
  }

  return all_required_args_provided && !has_invalid_arguments_ ? 0 : 1;
}

std::string ArgumentParser::typeAsString(DataType t) const {
  if (t == INT)      return "INTEGER";
  if (t == BOOL)     return "BOOLEAN";
  if (t == DOUBLE)   return "DOUBLE";
  if (t == STRING)   return "STRING";

  return "";
}

void ArgumentParser::handleInvalidArgument(const string& flag) {
  std::cout << BOLDRED << "Invalid argument " << flag << ": [" 
            << typeAsString(args_[flag].data_type_) << "] "
            << args_[flag].description_ << RESET << std::endl;

  has_invalid_arguments_ = true;
}
