#ifndef HTML_ASSETS
#define HTML_ASSETS

#include <string>

namespace html_assets {

// the html to return when a markdown/edit request is made
// "sffs" is used as filler text    
const std::string edit_form = "<form action=\"/markdown/save\" method=\"POST\" accept-charset=\"utf-8\" enctype=\"text/plain\">\n"
    "  Markdown:<br>\n"
    "  <input type=\"hidden\" name=\"postid\" value=\"editsPID\">\n"
    "  <textarea name=\"text\">sffs</textarea><br>\n"
    "  <input type=\"submit\" id=\"submit-button\" value=\"Save\">\n"
    "</form>\n"
    "<form action= \"/markdown/delete\"  method=\"POST\">\n"
    "  <input type=\"hidden\" name=\"postid\" value=\"editsPID\">\n"
    "  <input type=\"submit\" value=\"Delete\">\n"
    "</form>";

const std::string list_table_start = "<table><tbody>";
const std::string list_row_start = "<tr><td>";
const std::string list_row_mid = "</td><td>";
const std::string list_row_end = "</td></tr>";
const std::string list_line_form_a = "<form action=\"/markdown/view\" method=\"GET\" accept-charset=\"utf-8\" enctype=\"text/plain\">"
    "<input type=\"hidden\" name=\"postid\" value=\"";
const std::string list_line_form_b = "\"><input type=\"submit\" value=\"View\">"
    "</form>";
const std::string list_table_end = "<tr><td><form action=\"/markdown/create\" method=\"GET\">"
    "<input type=\"submit\" value=\"New post\"></form></td></tr></tbody></table>";

const std::string edit_button = "<form action=\"/markdown/edit\" method=\"POST\" accept-charset=\"utf-8\" enctype=\"text/plain\">"
    "<input type=\"hidden\" name=\"postid\" value=\"sffs\"><input type=\"submit\" value=\"Edit\">"
    "</form>";
const std::string list_button = "<form action=\"/markdown/list\" method=\"GET\"><input type=\"submit\" value=\"List posts\"></form>";
}

#endif // HTML_ASSETS
