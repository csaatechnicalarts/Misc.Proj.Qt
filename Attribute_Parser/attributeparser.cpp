#include "attributeparser.h"
#include <iostream>
#include <regex>
#include <QDebug>

void ap::AttributeParser::GetInfo()
{
    for(const TagNode& x: m_vecTagNode) {
        qInfo() << "(" << x.getNameQualifier() << ") " << x.getTag_name() << " - ";
        for(const AttributeNode& y: x.getAttributes()) {
            qInfo() << y.getAttrib_name() << ": " << y.getAttrib_val() << " ";
        }
        qInfo() << "";
    }
}

void ap::AttributeParser::ParseHRML()
{
    char c{};
    std::string tok{}, tag{}, c_eq{};
    std::smatch m_res_rec, m_res_sub;
    std::regex regex_record ("[_A-Za-z0-9]*\\s*=\\s*[\"._A-Za-z0-9%!$]*");
    std::regex regex_subrecord ("\\s*=\\s*");
    std::vector<std::string> namePathStack;

    for (int i = 0; i < m_lines; ++i) {
        TagNode tn{};
        tok = m_console->readLine().toStdString();

        if (tok[1] != '/') {
            // Discard the leading '<' and trailing '>'
            tok.erase(0, 1);
            tok.erase(tok.size() - 1);
            std::stringstream(tok) >> tag;
            tok.erase(0, tag.length());

            namePathStack.push_back(tag);

            std::vector<AttributeNode> v_attributes{};
            while (std::regex_search(tok, m_res_rec, regex_record)) {
                AttributeNode attrib_node{};
                for (auto& x: m_res_rec) {
                    //std::cout << x << " ";
                    std::string expectsString(x);
                    std::regex_search(expectsString, m_res_sub, regex_subrecord);
                    attrib_node.setAttrib_name(m_res_sub.prefix());
                    // Strip surrounding quotation marks.
                    std::string av = m_res_sub.suffix();
                    av.erase(0,1);
                    av.erase(av.size() -1);
                    attrib_node.setAttrib_val(av);
                    v_attributes.push_back(attrib_node);
                }

                tok = m_res_rec.suffix().str();
            }

            tn.setTag_name(tag);
            tn.setAttributes(v_attributes);
            // The name path is the dotted qualifier of this attribute
            std::string name_qualifier{};
            for (auto it = namePathStack.begin(); it != (namePathStack.end() - 1); ++it) {
                name_qualifier.append(*it + ".");
            }
            tn.setNameQualifier(name_qualifier);
            m_vecTagNode.push_back(tn);
        } else {
            namePathStack.pop_back();
        }
    }
}


void ap::AttributeParser::GetParams()
{
    QString param;

    // The first input line is alwasy an integer N, followed by white space, then by integer Q.
    param = m_console->readLine();
    m_ssparams.str(param.toStdString());
    m_ssparams >> m_lines >> m_queries;
}

void ap::AttributeParser::ParseQueries() {
    std::smatch m_q{};
    std::smatch m_subq{};
    std::string query{};
    std::regex regex_query ("\\.*[_A-Za-z0-9]+~[_A-Za-z0-9]+");
    std::regex regex_subquery ("~");
    bool tagMatch, attribMatch;
    std::string tagFound, attributeFound, valueFound;

    for(int i = 0; i < m_queries; ++i) {
        tagMatch = attribMatch = false;
        query = m_console->readLine().toStdString();

        if (std::regex_search(query, m_q, regex_query)) {
            // Disregard leading '.'
            std::string query_sub = std::string(std::string(m_q[0]));
            if(query_sub[0] == '.')
                query_sub.erase(0, 1);
            std::regex_search(query_sub, m_subq, regex_subquery);

            for (auto it = m_vecTagNode.begin(); it != m_vecTagNode.end(); ++it) {
                if (it->getTag_name() == m_subq.prefix()){
                    if (!IsValidNamePath(it->getTag_name(), m_q.prefix()))
                        break;
                    tagFound = it->getTag_name();
                    tagMatch = true;

                    std::vector<AttributeNode> xt = it->getAttributes();
                    std::vector<AttributeNode>::iterator jt = xt.begin();

                    for (; jt != xt.end(); ++jt) {
                        if (jt->getAttrib_name() == m_subq.suffix()) {
                            attributeFound = jt->getAttrib_name();
                            valueFound = jt->getAttrib_val();
                            attribMatch = true;
                            break;
                        }
                    }
                }
            }
        }

        if (tagMatch && attribMatch) {
            qInfo() << valueFound;
        } else {
            qInfo() << "Not Found!";
        }
    }
}

bool ap::AttributeParser::IsValidNamePath(const std::string tag, std::string name_path) {
    std::vector<TagNode>::const_iterator it;
    for(it = m_vecTagNode.begin(); it != m_vecTagNode.end(); ++it) {
        if (it->getTag_name() == tag)
            break;
    }

    if (name_path != "")
        name_path.append(".");

    return it->getNameQualifier() == name_path;
}
