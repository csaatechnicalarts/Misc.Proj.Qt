#ifndef ATTRIBUTEPARSER_H
#define ATTRIBUTEPARSER_H

#include <QObject>
#include <QTextStream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <mutex>

namespace ap {
        class AttributeNode {
        private:
            std::string         m_attrib_name;
            std::string         m_attrib_val;

        public:
            AttributeNode(): m_attrib_name{}, m_attrib_val{} {}
            AttributeNode(const std::string& an, const std::string& av):
                m_attrib_name(an), m_attrib_val(av) {}

            std::string getAttrib_name() const {
                return m_attrib_name;
            }
            void setAttrib_name(const std::string &newAttrib_name) {
                m_attrib_name = newAttrib_name;
            }

            std::string getAttrib_val() const {
                return m_attrib_val;
            }
            void setAttrib_val(const std::string &newAttrib_val) {
                m_attrib_val = newAttrib_val;
            }
        };

        class TagNode {
        private:
            std::string                         m_tag_name;
            std::vector<struct AttributeNode>   m_attributes;
            std::string                         m_name_qualifier;

        public:
            TagNode(): m_tag_name{}, m_attributes{}, m_name_qualifier{} {}

            std::string getTag_name() const { return m_tag_name; }

            void setTag_name(const std::string &newTag_name) {
                m_tag_name = newTag_name;
            }

            void setAttributes(const std::vector<struct AttributeNode>& v_an) {
                m_attributes = v_an;
            }

            void pushBack(const struct AttributeNode& at) {
                m_attributes.push_back(at);
            }

            std::vector<AttributeNode> getAttributes() const { return m_attributes; }

            std::string getNameQualifier() const { return m_name_qualifier; }

            void setNameQualifier(const std::string &newNameQualifier) {
                m_name_qualifier = newNameQualifier;
            }
        };

        class AttributeParser
        {
        private:
            std::vector<TagNode>        m_vecTagNode{};
            std::stringstream           m_ssparams{};
            int                         m_lines{};
            int                         m_queries{};
            QTextStream*                m_console{};

            bool IsValidNamePath(const std::string tag, std::string name_path);

            // Singleton-related members
            static AttributeParser*     m_singletonPtr;
            static std::mutex           m_mutex;

            AttributeParser() {
                m_console = new QTextStream(stdin);
            }
        public:
            ~AttributeParser() {
                delete m_console;
            }

            void GetParams();
            void ParseHRML();
            void ParseQueries();
            void GetInfo();

            // Singleton-related: Prevent copy constructor
            AttributeParser(const AttributeParser& other) = delete;

            // Singleton-related: Factory method
            static AttributeParser* getInstance() {
                if(m_singletonPtr == nullptr) {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    if (m_singletonPtr == nullptr) {
                        m_singletonPtr = new AttributeParser();
                    }
                }

                return m_singletonPtr;
            }
        };
}
#endif // ATTRIBUTEPARSER_H
