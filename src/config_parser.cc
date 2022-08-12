// An nginx config file parser.
//
// See:
//   http://wiki.nginx.org/Configuration
//   http://blog.martinfjordvald.com/2010/07/nginx-primer/
//
// How Nginx does it:
//   http://lxr.nginx.org/source/src/core/ngx_conf_file.c

#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>
#include <ctype.h>
#include <cstdlib>

#include "config_parser.h"
#include "logging.h"

std::string NginxConfig::ToString(int depth) {
  std::string serialized_config;
  for (const auto& statement : statements_) {
    serialized_config.append(statement->ToString(depth));
  }
  return serialized_config;
}

std::string NginxConfigStatement::ToString(int depth) {
  std::string serialized_statement;
  for (int i = 0; i < depth; ++i) {
    serialized_statement.append("  ");
  }
  for (unsigned int i = 0; i < tokens_.size(); ++i) {
    if (i != 0) {
      serialized_statement.append(" ");
    }
    serialized_statement.append(tokens_[i]);
  }
  if (child_block_.get() != nullptr) {
    serialized_statement.append(" {\n");
    serialized_statement.append(child_block_->ToString(depth + 1));
    for (int i = 0; i < depth; ++i) {
      serialized_statement.append("  ");
    }
    serialized_statement.append("}");
  } else {
    serialized_statement.append(";");
  }
  serialized_statement.append("\n");
  return serialized_statement;
}

const char* NginxConfigParser::TokenTypeAsString(TokenType type) {
  switch (type) {
    case TOKEN_TYPE_START:         return "TOKEN_TYPE_START";
    case TOKEN_TYPE_NORMAL:        return "TOKEN_TYPE_NORMAL";
    case TOKEN_TYPE_START_BLOCK:   return "TOKEN_TYPE_START_BLOCK";
    case TOKEN_TYPE_END_BLOCK:     return "TOKEN_TYPE_END_BLOCK";
    case TOKEN_TYPE_COMMENT:       return "TOKEN_TYPE_COMMENT";
    case TOKEN_TYPE_STATEMENT_END: return "TOKEN_TYPE_STATEMENT_END";
    case TOKEN_TYPE_EOF:           return "TOKEN_TYPE_EOF";
    case TOKEN_TYPE_ERROR:         return "TOKEN_TYPE_ERROR";
    default:                       return "Unknown token type";
  }
}

NginxConfigParser::TokenType NginxConfigParser::ParseToken(std::istream* input,
                                                           std::string* value) {
  TokenParserState state = TOKEN_STATE_INITIAL_WHITESPACE;
  // trigger the state when encounter braces {
  bool inside_brace = false;

  while (input->good()) {
    const char c = input->get();
    if (!input->good()) {
      break;
    }
  
    switch (state) {
      case TOKEN_STATE_INITIAL_WHITESPACE:
        switch (c) {
          case '{':
            *value = c;
            return TOKEN_TYPE_START_BLOCK;
          case '}':
            *value = c;
            return TOKEN_TYPE_END_BLOCK;
          case '#':
            *value = c;
            state = TOKEN_STATE_TOKEN_TYPE_COMMENT;
            continue;
          case '"':
            *value = c;
            state = TOKEN_STATE_DOUBLE_QUOTE;
            continue;
          case '\'':
            *value = c;
            state = TOKEN_STATE_SINGLE_QUOTE;
            continue;
          case ';':
            *value = c;
            return TOKEN_TYPE_STATEMENT_END;
          case ' ':
          case '\t':
          case '\n':
          case '\r':
            continue;
          default:
            *value += c;
            state = TOKEN_STATE_TOKEN_TYPE_NORMAL;
            continue;
        }
      case TOKEN_STATE_SINGLE_QUOTE:
        // the end of a quoted token should be followed by whitespace.
        // allow for backslash-escaping within strings.
        *value += c;

        if (c == '\'') {

          // check next char and put back after
          const char check_space = input->get();
          input->unget();

          if (check_space == ' ' || check_space == '\t' || check_space == '\n' || 
              check_space == '\t' || check_space == ';' )
            return TOKEN_TYPE_NORMAL;

          else if ((*value)[(*value).size() - 2] == '\\') // escape quote
            value->erase((*value).size() - 2, 1); // erase "\"

          else
            return TOKEN_TYPE_ERROR;
        }

        else if (c == '\\') {

          // check next char and put back after
          const char check_backslash = input->get();
          input->unget();

          if (check_backslash == '\\')
            value->pop_back(); // erase one "\"

        }
        
        continue;
      case TOKEN_STATE_DOUBLE_QUOTE:
        *value += c;

        if (c == '"') {

          // check next char and put back after
          const char check_space = input->get();
          input->unget();
          
          if (check_space == ' ' || check_space == '\t' || check_space == '\n' || 
              check_space == '\t' || check_space == ';' )
            return TOKEN_TYPE_NORMAL;

          else if ((*value)[(*value).size() - 2] == '\\') // escape quote
            value->erase((*value).size() - 2, 1); // erase "\"
          else
            return TOKEN_TYPE_ERROR;
        }

        else if (c == '\\') {

          // check next char and put back after
          const char check_backslash = input->get();
          input->unget();

          if (check_backslash == '\\')
            value->pop_back(); // erase one "\"

        }

        continue;
      case TOKEN_STATE_TOKEN_TYPE_COMMENT:
        if (c == '\n' || c == '\r') {
          return TOKEN_TYPE_COMMENT;
        }
        *value += c;
        continue;
      case TOKEN_STATE_TOKEN_TYPE_NORMAL:
        if (c == ' ' || c == '\t' || c == '\n' || c == '\t' ||
            c == ';' || c == '{' || c == '}') {
          input->unget();
          return TOKEN_TYPE_NORMAL;
        }
        *value += c;
        continue;
    }
  }

  // If we get here, we reached the end of the file.
  if (state == TOKEN_STATE_SINGLE_QUOTE ||
      state == TOKEN_STATE_DOUBLE_QUOTE) {
    return TOKEN_TYPE_ERROR;
  }

  return TOKEN_TYPE_EOF;
}

bool NginxConfigParser::Parse(std::istream* config_file, NginxConfig* config) {
  std::stack<NginxConfig*> config_stack;
  config_stack.push(config);
  TokenType last_token_type = TOKEN_TYPE_START;
  TokenType token_type;
  while (true) {
    std::string token;
    token_type = ParseToken(config_file, &token);
    // log_token(TokenTypeAsString(token_type), token);
    if (token_type == TOKEN_TYPE_ERROR) {
      break;
    }

    if (token_type == TOKEN_TYPE_COMMENT) {
      // Skip comments.
      continue;
    }

    if (token_type == TOKEN_TYPE_START) {
      // Error.
      break;
    } else if (token_type == TOKEN_TYPE_NORMAL) {
      if (last_token_type == TOKEN_TYPE_START ||
          last_token_type == TOKEN_TYPE_STATEMENT_END ||
          last_token_type == TOKEN_TYPE_START_BLOCK ||
          last_token_type == TOKEN_TYPE_END_BLOCK ||
          last_token_type == TOKEN_TYPE_NORMAL) {
        if (last_token_type != TOKEN_TYPE_NORMAL) {
          config_stack.top()->statements_.emplace_back(
              new NginxConfigStatement);
        }
        config_stack.top()->statements_.back().get()->tokens_.push_back(
            token);
      } else {
        // Error.
        break;
      }
    } else if (token_type == TOKEN_TYPE_STATEMENT_END) {
      if (last_token_type != TOKEN_TYPE_NORMAL) {
        // Error.
        break;
      }
    } else if (token_type == TOKEN_TYPE_START_BLOCK) {
      if (last_token_type != TOKEN_TYPE_NORMAL) {
        // Error.
        break;
      }
      NginxConfig* const new_config = new NginxConfig;
      config_stack.top()->statements_.back().get()->child_block_.reset(
          new_config);
      config_stack.push(new_config);
    } else if (token_type == TOKEN_TYPE_END_BLOCK) {
      if (last_token_type != TOKEN_TYPE_STATEMENT_END &&
          last_token_type != TOKEN_TYPE_START_BLOCK &&
          last_token_type != TOKEN_TYPE_END_BLOCK) {
        // Error.
        break;
      }
      config_stack.pop();
    } else if (token_type == TOKEN_TYPE_EOF) {
      if (last_token_type != TOKEN_TYPE_STATEMENT_END &&
          last_token_type != TOKEN_TYPE_END_BLOCK) {
        // Error.
        break;
      }
      return true;
    } else {
      // Error. Unknown token.
      break;
    }
    last_token_type = token_type;
  }

  // log_bad_transition(TokenTypeAsString(last_token_type), TokenTypeAsString(token_type));
  return false;
}

bool NginxConfigParser::Parse(const char* file_name, NginxConfig* config) {
  std::ifstream config_file;
  config_file.open(file_name);
  if (!config_file.good()) {
    // log_bad_file(file_name);
    return false;
  }

  const bool return_value =
      Parse(dynamic_cast<std::istream*>(&config_file), config);
  config_file.close();
  return return_value;
}

int NginxConfigParser::ExtractPort(const char* file_name) {
  std::ifstream config_file;
  config_file.open(file_name);

  while (config_file.good()) {

    //read the char inside the file
    const char c = config_file.get();

    if (!config_file.good()) {
      break;
    }

    switch(c) {
      case 'l':
      case 'L':
      case 'p':
      case 'P': {   
        std::string token_listen = "isten ";
        std::string token_port = "ort ";

        std::string token_str = "";

        const char option = tolower(c);

        if (option == 'l') {
          // read the next 6 chars
          for (int i = 0; i < 6; ++i) {
            token_str = token_str + static_cast<char> (config_file.get());
          }
        } 
        else {
          // read the next 4 chars
          for (int i = 0; i < 4; ++i) {
            token_str = token_str + static_cast<char> (config_file.get());
          }
        }

        // check the token string with specific option
        if ( (option == 'l' && token_str== token_listen ) ||
             (option == 'p' && token_str == token_port) ) {
          std::string port_num = "";

          // clear the white space
          while (isspace(config_file.get())) {
            port_num = "";
          }
          
          config_file.unget(); // unget last char

          // try port number >= 1024, <= 65535
          port_num = "";
          char read_char = config_file.get();

          if (read_char == '0') {
            config_file.close();
            return -1;
          }
            

          else if (!isdigit(read_char)) {
            config_file.unget();
            continue;
          }
          
          else {
            int port_int_num = 0;

            // keep reading the digits
            while (isdigit(read_char)) {
              port_num += read_char;

              read_char = config_file.get();
            }

            port_int_num = stoi(port_num);

            // verify the ranges
            if ( (port_int_num >= 1024 && 
                  port_int_num <= 65535) || port_int_num == 80 ) {
                config_file.close();
                // 2^16 -1
                return port_int_num;
            }
            else { 
              config_file.close();
              return -1;
            }
          }        
        }

        // if the port number not extracted/valid
        if (option == 'l') {
          // unget the prev 6 chars
          for (int i = 0; i < 6; ++i) {
            config_file.unget();
          }
        } 
        else {
          // unget the prev 4 chars
          for (int i = 0; i < 4; ++i) {
            config_file.unget();
          }
        }
        continue;
      }
      default:
        continue;
    }
  }

  config_file.close();
  return -1;
}

location_info NginxConfigParser::ExtractLocation(const char* file_name) {
  std::ifstream config_file;
  config_file.open(file_name);

  // trigger the state when encounter brace {
  bool inside_brace = false;
  bool valid_location = false;
  std::string path_name = "";
  std::string handler_name = "";
  std::string dir_name = "";
  std::string index_name = "";
  location_info loc_info;

  while (config_file.good()) {

    //read the char inside the file
    const char c = config_file.get();

    if (!config_file.good()) {
      break;
    }

    switch(c) {
      case '{':
        inside_brace = true;
        continue;
      case '}':
        inside_brace = false;
        continue;
      case '/':{

        // if followed by the "location" token
        if (valid_location) {
          path_name = "/";
          char read_char;

          read_char = config_file.get(); 
          // read the path for the handler
          // clear the white spaces
          while (!isspace(read_char)) {
            path_name += read_char;
            read_char = config_file.get();
          }

          valid_location = false; // reset the condition for token "location"

          // clear the white space first and try to grab the handler after
          // clear the white spaces
          while (isspace(read_char)) {
            handler_name = "";
            read_char = config_file.get();
          }

          config_file.unget();

          read_char = config_file.get();

          while (isalpha(read_char) || isdigit(read_char))  { // letter or number for name
            handler_name += read_char;
            read_char = config_file.get();
          }

          config_file.unget();

          // log_debug("add entry to path_to_handler: <" + path_name + "," + handler_name + ">");
          loc_info.path_to_handler[path_name] = handler_name; // store in map
        }
        continue;
      }
      case 'l': 
      case 'L': {
        std::string token_location = "ocation ";
        std::string token_str = "";

        // read the next 8 chars
        for (int i = 0; i < 8; ++i) {
          token_str = token_str + static_cast<char> (config_file.get());
        }

        if (token_str == token_location) {
          valid_location = true;
        } 

        else {
          // unget the 8 chars
          for (int i = 0; i < 8; ++i) {
            config_file.unget();
          }
        }
        continue;
      }
      case 'r':
      case 'R': {
        if (inside_brace && path_name != "") { // if path_name is ready
          std::string token_root = "oot ";
          std::string token_str = "";
          dir_name = "";
          
          // read the next 4 chars
          for (int i = 0; i < 4; ++i) {
            token_str = token_str + static_cast<char> (config_file.get());
          }

          if (token_str == token_root) {
            char dir_char;   // char for the base directory
            
            dir_char = config_file.get();
            // clear the white spaces or quote "
            while (isspace(dir_char) || dir_char == '\"') {
              dir_name= "";
              dir_char = config_file.get();
            }

            config_file.unget();

            dir_char = config_file.get();

            // if not ; not " and not spaces
            while (dir_char != ';' && !isspace(dir_char) && 
                   dir_char != '\"' && dir_char != '}') {
              dir_name = dir_name + dir_char;

              dir_char = config_file.get();
            }

            config_file.unget();

            loc_info.path_to_root[path_name] = dir_name;
          }

          // token root doesn't match
          else { 
            for (int i = 0; i < 4; ++i) {
              config_file.unget();
            }
          }
        }
        continue;
      }
      case 'i':
      case 'I': {
        if (inside_brace && path_name != "") {  // if path name is ready
          std::string token_index = "ndex ";
          std::string token_str = "";

          // read the next 5 chars
          for (int i = 0; i < 5; ++i) {
            token_str = token_str + static_cast<char> (config_file.get());
          }

          if (token_str == token_index) {
            // clear the white spaces
            while (isspace(config_file.get())) {
               index_name= "";
            }

            config_file.unget();

            char read_char = config_file.get();

            // if not ; and space
            while (read_char != ';' && !isspace(read_char) && read_char != '}') {
              index_name = index_name + read_char;

              read_char = config_file.get();
            }

            loc_info.path_to_index[path_name] = index_name;
          }

          // token index doesn't match
          else { 
            for (int i = 0; i < 5; ++i) {
              config_file.unget();
            }
          }
        }
        continue;
      }
      default:
        continue;
    }

  }

  config_file.close();
  return loc_info;
}
