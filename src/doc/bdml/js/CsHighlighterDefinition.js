//
// BDML Javascript file
//
// Copyright 2007 Bora Software (contact@borasoftware.com)
// https://borasoftware.com/specifications/bdml.html
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see https://www.gnu.org/licenses/.
//

// Syntax highlighter definition for C# code.

function CsHighlighterDefinition() {}

CsHighlighterDefinition.prototype.LANGUAGE_NAME = "C#";

// TODO API comment blocks
CsHighlighterDefinition.COMMENT_BLOCK_START = BdmlXRegExp("^/\\*");
CsHighlighterDefinition.COMMENT_BLOCK_END = BdmlXRegExp("\\*/");

CsHighlighterDefinition.COMMENT_LINE = BdmlXRegExp("^\/\/.*");

CsHighlighterDefinition.NAME = new BdmlXRegExp("^[\\p{L}_][\\p{L}0-9_]*");

CsHighlighterDefinition.KEYWORDS = BdmlXRegExp("^(abstract|as|base|break|by|case|catch|checked|class|const|continue|default|delegate|do|descending|explicit|event|extern|else|enum|false|finally|fixed|for|foreach|from|goto|group|if|implicit|in|interface|internal|into|is|lock|new|null|namespace|operator|out|override|orderby|params|private|protected|public|readonly|ref|return|switch|struct|sealed|sizeof|stackalloc|static|select|this|throw|true|try|typeof|unchecked|unsafe|using|virtual|volatile|while|where|yield)(?![\\p{L}0-9_])");

CsHighlighterDefinition.TYPES = BdmlXRegExp("^(bool|byte|char|decimal|double|float|int|long|object|sbyte|short|string|uint|ulong|ushort|var|void)(?![\\p{L}0-9_])");

CsHighlighterDefinition.prototype.PATTERNS = Object.freeze({
	STRING_ARRAY:            HighlighterPatterns.STRING_ARRAY
	, STRING_CONSTANT:       HighlighterPatterns.STRING_CONSTANT
	, NEWLINE:               HighlighterPatterns.NEWLINE
	, WHITESPACE:            HighlighterPatterns.WHITESPACE
	, COMMENT_BLOCK_START:   CsHighlighterDefinition.COMMENT_BLOCK_START
	, COMMENT_LINE:          CsHighlighterDefinition.COMMENT_LINE
	, OPEN_SQUARE:           HighlighterPatterns.OPEN_SQUARE
	, CLOSE_SQUARE:          HighlighterPatterns.CLOSE_SQUARE
	, OPEN_PARENTHESIS:      HighlighterPatterns.OPEN_PARENTHESIS
	, CLOSE_PARENTHESIS:     HighlighterPatterns.CLOSE_PARENTHESIS
	, OPEN_CURLY:            HighlighterPatterns.OPEN_CURLY
	, CLOSE_CURLY:           HighlighterPatterns.CLOSE_CURLY
	, DOT:                   HighlighterPatterns.DOT
	, SEMICOLON:             HighlighterPatterns.SEMICOLON
	, COMMA:                 HighlighterPatterns.COMMA
	, COLON:                 HighlighterPatterns.COLON
	, LAMBDA:                HighlighterPatterns.LAMBDA
	, KEYWORD:               CsHighlighterDefinition.KEYWORDS
	, TYPE_NAME:             CsHighlighterDefinition.TYPES
	, NAME:                  CsHighlighterDefinition.NAME
	, NUMBER:                HighlighterPatterns.NUMBER
	, HEX_NUMBER:            HighlighterPatterns.HEX_NUMBER
	, OCTAL_NUMBER:          HighlighterPatterns.OCTAL_NUMBER
	, EQUALS:                HighlighterPatterns.EQUALS
	, LESS_THAN:             HighlighterPatterns.LESS_THAN
	, GREATER_THAN:          HighlighterPatterns.GREATER_THAN
	, LESS_THAN_OR_EQUAL:    HighlighterPatterns.LESS_THAN_OR_EQUAL
	, GREATER_THAN_OR_EQUAL: HighlighterPatterns.GREATER_THAN_OR_EQUAL
	, EXCLAMATION_EQUAL:     HighlighterPatterns.EXCLAMATION_EQUAL
	, EQUAL:                 HighlighterPatterns.EQUAL
	, PLUS_EQUALS:           HighlighterPatterns.PLUS_EQUALS
	, MINUS_EQUALS:          HighlighterPatterns.MINUS_EQUALS
	, MULTIPLY_EQUALS:       HighlighterPatterns.MULTIPLY_EQUALS
	, DIVIDE_EQUALS:         HighlighterPatterns.DIVIDE_EQUALS
	, AND_EQUALS:            HighlighterPatterns.AND_EQUALS
	, OR_EQUALS:             HighlighterPatterns.OR_EQUALS
	, PLUS_PLUS:             HighlighterPatterns.PLUS_PLUS
	, MINUS_MINUS:           HighlighterPatterns.MINUS_MINUS
	, PLUS:                  HighlighterPatterns.PLUS
	, MINUS:                 HighlighterPatterns.MINUS
	, EXCLAMATION:           HighlighterPatterns.EXCLAMATION
	, MULTIPLY:              HighlighterPatterns.MULTIPLY
	, DIVIDE:                HighlighterPatterns.DIVIDE
	, AND_AND:               HighlighterPatterns.AND_AND
	, OR_OR:                 HighlighterPatterns.OR_OR
	, AND:                   HighlighterPatterns.AND
	, OR:                    HighlighterPatterns.OR
	, QUESTION_MARK:         HighlighterPatterns.QUESTION_MARK
	, SHIFT_LEFT_EQUALS:     HighlighterPatterns.SHIFT_LEFT_EQUALS
	, SHIFT_RIGHT3_EQUALS:   HighlighterPatterns.SHIFT_RIGHT3_EQUALS
	, SHIFT_RIGHT_EQUALS:    HighlighterPatterns.SHIFT_RIGHT_EQUALS
	, SHIFT_LEFT:            HighlighterPatterns.SHIFT_LEFT
	, SHIFT_RIGHT3:          HighlighterPatterns.SHIFT_RIGHT3
	, SHIFT_RIGHT:           HighlighterPatterns.SHIFT_RIGHT
	, ANNOTATION:            HighlighterPatterns.ANNOTATION
});

CsHighlighterDefinition.prototype.ELEMENT_NAMES = {};

CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.UNKNOWN] =                HighlighterElementNames.UNKNOWN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.STRING_ARRAY] =           HighlighterElementNames.STRING;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.STRING_CONSTANT] =        HighlighterElementNames.STRING;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.NEWLINE] =                HighlighterElementNames.EMPTY;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.WHITESPACE] =             HighlighterElementNames.EMPTY;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.APIDOC] =                 HighlighterElementNames.API_DOC;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_LINE] =           HighlighterElementNames.COMMENT;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_BLOCK_START] =    HighlighterElementNames.COMMENT;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_BLOCK_TEXT] =     HighlighterElementNames.COMMENT;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_BLOCK_END] =      HighlighterElementNames.COMMENT;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OPEN_SQUARE] =            HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CLOSE_SQUARE] =           HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OPEN_PARENTHESIS] =       HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CLOSE_PARENTHESIS] =      HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OPEN_CURLY] =             HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CLOSE_CURLY] =            HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.DOT] =                    HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SEMICOLON] =              HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMA] =                  HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COLON] =                  HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.LAMBDA] =                 HighlighterElementNames.LAMBDA;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.KEYWORD] =                HighlighterElementNames.KEYWORD;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.TYPE_NAME] =              HighlighterElementNames.TYPE_IDENTIFIER;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.NAME] =                   HighlighterElementNames.IDENTIFIER;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.NUMBER] =                 HighlighterElementNames.NUMBER;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.HEX_NUMBER] =             HighlighterElementNames.HEX_OCT_NUMBER;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OCTAL_NUMBER] =           HighlighterElementNames.HEX_OCT_NUMBER;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.BINARY_NUMBER] =          HighlighterElementNames.HEX_OCT_NUMBER;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_NUMBER] =            HighlighterElementNames.NUMBER;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_HEX_NUMBER] =        HighlighterElementNames.HEX_OCT_NUMBER;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_OCTAL_NUMBER] =      HighlighterElementNames.HEX_OCT_NUMBER;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_BINARY_NUMBER] =     HighlighterElementNames.HEX_OCT_NUMBER;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.QUESTION_MARK] =          HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.UNDERSCORE] =             HighlighterElementNames.UNDERSCORE;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EQUALS] =                 HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.LESS_THAN] =              HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.GREATER_THAN] =           HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.LESS_THAN_OR_EQUAL] =     HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.GREATER_THAN_OR_EQUAL] =  HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EXCLAMATION_EQUAL] =      HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EQUAL] =                  HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.AND] =                    HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OR] =                     HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.AND_AND] =                HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OR_OR] =                  HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.PLUS_EQUALS] =            HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MINUS_EQUALS] =           HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MULTIPLY_EQUALS] =        HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.DIVIDE_EQUALS] =          HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.AND_EQUALS] =             HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OR_EQUALS] =              HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.PLUS_PLUS] =              HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MINUS_MINUS] =            HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.PLUS] =                   HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MINUS] =                  HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EXCLAMATION] =            HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MULTIPLY] =               HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.DIVIDE] =                 HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_LEFT_EQUALS] =      HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_RIGHT3_EQUALS] =    HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_RIGHT_EQUALS] =     HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_RIGHT3] =           HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_RIGHT] =            HighlighterElementNames.PLAIN;
CsHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.ANNOTATION] =             HighlighterElementNames.ANNOTATION;

Object.freeze(CsHighlighterDefinition.prototype.ELEMENT_NAMES);

CsHighlighterDefinition.prototype.getName = function () {
	return this.LANGUAGE_NAME;
};

CsHighlighterDefinition.prototype.hasPrompts = function () {
	return false;
};

CsHighlighterDefinition.prototype.prompt1 = function () {
	return null;
};

CsHighlighterDefinition.prototype.prompt2 = function () {
	return null;
};

CsHighlighterDefinition.prototype.getCommentBlockType = function () {
	return SimpleHighlighterDefinition.CommentBlockType.NON_RECURSIVE;
};

CsHighlighterDefinition.prototype.commentBlockStart = function () {
	return CsHighlighterDefinition.COMMENT_BLOCK_START;
};

CsHighlighterDefinition.prototype.commentBlockEnd = function () {
	return CsHighlighterDefinition.COMMENT_BLOCK_END;
};

CsHighlighterDefinition.prototype.patterns = function () {
	return this.PATTERNS;
};

CsHighlighterDefinition.prototype.elementNames = function () {
	return this.ELEMENT_NAMES;
};

CsHighlighterDefinition.prototype.backslashIsLineContinuation = function () {
	return false;
};

CsHighlighterDefinition.prototype.preHighlightingProcessing = function (input) {
	return input;
};