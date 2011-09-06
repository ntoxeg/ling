/**
 * @file    parser.hpp
 * @author  Wintermute Developers <wintermute-devel@lists.launchpad.net>
 * @date    June 14, 2011 11:34 PM
 * @license GPL3
 *
 * @legalese
 * Copyright (c) SII 2010 - 2011
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * @endlegalese
 */

#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <string>
#include <map>
#include <QString>
#include <QStringList>
#include <wntr/data/models.hpp>
#include "syntax.hpp"

using namespace std;

using std::string;

namespace Wintermute {
    namespace Linguistics {
        struct Rule;
        struct Parser;
        struct Binding;
        struct Meaning;

        /**
         * @brief Represents a vector of vector of nodes.
         * @typedef NodeTree
         */
        typedef QList<NodeVector> NodeTree;

        /**
         * @brief Represents a collection of meanings.
         * @typedef MeaningVector
         */
        typedef QList<Meaning*> MeaningVector;

        /**
         * @brief
         * @typedef RuleList
         */
        typedef QList<Rule*> RuleVector;

        /**
         * @brief
         *
         * @typedef BindingVector
         */
        typedef QList<Binding*> BindingVector;

        /**
         * @brief Represents the potential connection of words by a specified rule as defined by its parent rule.
         * @class Binding parser.hpp "include/wntr/ling/parser.hpp"
         */
        class Binding : public QObject {
            Q_OBJECT
            friend class Rule;
            signals:
                /**
                 * @brief
                 * @fn binded
                 * @param
                 * @param
                 * @param
                 */
                void binded(const Binding* = NULL, const Node* = NULL, const Node* = NULL) const;
                /**
                 * @brief
                 * @fn bindFailed
                 * @param
                 * @param
                 * @param
                 */
                void bindFailed(const Binding* = NULL, const Node* = NULL, const Node* = NULL) const;

            public:
                /**
                 * @brief
                 * @fn Binding
                 */
                explicit Binding();
                /**
                 * @brief
                 * @fn Binding
                 * @param p_bnd
                 */
                Binding(const Binding& );
                /**
                 * @brief
                 * @fn ~Binding
                 */
                ~Binding() { }
                /**
                 * @brief
                 *
                 * @fn obtain
                 * @param
                 * @param
                 */
                static const Binding* obtain ( const Node&, const Node& );
                /**
                 * @brief
                 * @fn parentRule
                 */
                const Rule* parentRule() const;
                /**
                 * @brief The ability of binding is measured on a scale from 0.0 to 1.0; where 0.0 is no chance at all and 1.0 is equality.
                 * @fn canBind
                 * @param
                 * @param
                 */
                const double canBind ( const Node&, const Node& ) const;
                /**
                 * @brief
                 * @fn getAttrValue
                 * @param
                 */
                const QString getAttrValue ( const QString& ) const;
                /**
                 * @brief
                 * @fn bind
                 * @param
                 * @param
                 */
                const Link* bind ( const Node&, const Node& ) const;

            protected:
                /**
                 * @brief
                 * @fn Binding
                 * @param QDomElement
                 * @param
                 */
                Binding ( const Rules::Bond& , const Rule* );

            private:
                Rules::Bond m_bnd;
                const Rule* m_rl;
        };

        /**
         * @brief Represents a set of bindings that permit linguistics links to be converted into ontological links.
         * @class Rule parser.hpp "include/wntr/ling/parser.hpp"
         */
        class Rule : public QObject {
            Q_OBJECT
            Q_PROPERTY(string type READ type)
            Q_PROPERTY(string locale READ locale)

            friend class RuleSet;
            public:
                virtual ~Rule() { }
                /**
                 * @brief Copy constructor.
                 * @fn Rule
                 * @param Rule The original Node.
                 */
                Rule ( const Rule& );
                /**
                 * @brief
                 *
                 * @fn Rule
                 * @param
                 */
                Rule ( const Rules::Chain& );
                /**
                 * @brief Empty constructor.
                 * @fn Rule
                 */
                explicit Rule( );
                /**
                 * @brief Returns a Rule that's satisified by this Node.
                 * @fn obtain
                 * @param Node A qualifying Node.
                 */
                static const Rule* obtain ( const Node & );
                /**
                 * @brief Determines if this Node can be binded with a Node that falls under this rule.
                 * @fn canBind
                 * @param Node The source Node in question.
                 * @param Node The destination Node in question.
                 */
                const bool canBind ( const Node & , const Node & ) const;
                /**
                 * @brief Creates a Link between a qualifying source Node and a destination Node.
                 * @fn bind
                 * @param Node The source Node in question.
                 * @param Node The destination Node in question.
                 */
                const Link* bind ( const Node &, const Node & ) const;
                /**
                 * @brief Returns a string representing the type of nodes that this Rule looks for.
                 * @fn type
                 */
                const string type() const;
                /**
                 * @brief
                 *
                 * @fn locale
                 */
                const string locale() const;
                /**
                 * @brief Determines if a Node is qualified to use this Rule.
                 * @fn appliesFor
                 * @param Node The source node in question.
                 */
                const double appliesFor ( const Node& ) const;
                /**
                 * @brief Returns a pointer to the Binding that works for the two specified Nodes.
                 * @fn getBindingFor
                 * @param Node The source Node in question.
                 * @param Node The destination Node in question.
                 */
                const Binding* getBindingFor ( const Node&, const Node& ) const;

            private:
                void __init();
                Rules::Chain m_chn;
                BindingVector m_bndVtr;
        };

        /**
         * @brief Encapsulates the primary object used to cast a simple string representing a bit of language into machine-interpretable ontological information.
         * @attention Using Big O notation, the parser avoids running into memory intenstive operations. The typical execution size is O(n1) * ...  * O(n(x)).
         *            Typically words have only one to three definitions. But if a sentence has words with 6 different meangins and there's 40 words; things get hairy <b>fast</b>.
         * @class Parser parser.hpp "include/wntr/ling/parser.hpp"
         */
        class Parser : public QObject {
            Q_OBJECT

            Q_PROPERTY(string locale READ locale WRITE setLocale)

            public:
                /**
                 * @brief
                 * @fn Parser
                 * @param p_prsr
                 */
                explicit Parser( const Parser& p_prsr ) : m_lcl(p_prsr.m_lcl) {}

                /**
                 * @brief
                 * @fn Parser
                 * @param
                 */
                Parser ( const string& = Wintermute::Data::Linguistics::Configuration::locale ()  );

                /**
                 * @brief
                 * @fn ~Parser
                 */
                ~Parser() { }

                /**
                 * @brief
                 * @fn locale
                 */
                const string locale() const;

                /**
                 * @brief
                 * @fn setLocale
                 * @param
                 */
                void setLocale ( const string& = Wintermute::Data::Linguistics::Configuration::locale ());

                /**
                 * @brief
                 * @fn parse
                 * @param
                 */
                void parse ( const string& );

            protected:
                mutable string m_lcl;

            private:
                /**
                 * @brief
                 * @fn process
                 * @param
                 */
                const Meaning* process ( const string& );
                /**
                 * @brief
                 * @fn getTokens
                 * @param
                 */
                QStringList getTokens ( string const & );
                /**
                 * @brief
                 * @fn formNode
                 * @param
                 */
                Node* formNode( QString const & );
                /**
                 * @brief
                 * @fn formNodes
                 * @param
                 */
                NodeVector formNodes ( QStringList const & );
                /**
                 * @brief
                 * @fn expandNodes
                 * @param
                 */
                NodeTree expandNodes ( NodeVector const & );
                /**
                 * @brief
                 * @fn expandNodes
                 * @param
                 * @param
                 * @param
                 */
                NodeTree expandNodes ( NodeTree& , const int& = 0, const int& = 0 );
                /**
                 * @brief
                 * @fn formMeaning
                 * @param
                 */
                const Meaning formMeaning ( const NodeVector& );
                /**
                 * @brief
                 * @fn formShorthand
                 * @param
                 * @param
                 */
                static const string formShorthand ( const NodeVector& , const Node::FormatVerbosity& = Node::FULL );

            private slots:
                void generateNode(Node*);

            signals:
                /**
                 * @brief
                 * @fn foundPseduoNode
                 * @param
                 */
                void foundPseduoNode(Node* = NULL);
                /**
                 * @brief
                 * @fn finishedTextAnalysis
                 */
                void finishedTextAnalysis();
                /**
                 * @brief
                 * @fn unwindingProgress
                 * @param
                 */
                void unwindingProgress(const double & = 0.0 );
                /**
                 * @brief
                 * @fn finishedUnwinding
                 */
                void finishedUnwinding();
                /**
                 * @brief
                 * @fn finishedMeaningForming
                 */
                void finishedMeaningForming();
        };
    }
}

Q_DECLARE_METATYPE(Wintermute::Linguistics::Parser)
Q_DECLARE_METATYPE(Wintermute::Linguistics::Binding)
Q_DECLARE_METATYPE(Wintermute::Linguistics::Rule)


#endif /* __PARSER_HPP__ */
// kate: indent-mode cstyle; space-indent on; indent-width 4;