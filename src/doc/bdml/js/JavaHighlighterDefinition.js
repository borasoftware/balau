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

// Syntax highlighter definition for Java code.

function JavaHighlighterDefinition() {}

JavaHighlighterDefinition.prototype.LANGUAGE_NAME = "JAVA";
// TODO API comment blocks
JavaHighlighterDefinition.COMMENT_BLOCK_START = BdmlXRegExp("^/\\*");
JavaHighlighterDefinition.COMMENT_BLOCK_END = BdmlXRegExp("\\*/");

JavaHighlighterDefinition.COMMENT_LINE = BdmlXRegExp("^\/\/.*");

JavaHighlighterDefinition.NAME = new BdmlXRegExp("^[\\p{L}_][\\p{L}0-9_]*");

JavaHighlighterDefinition.KEYWORDS = BdmlXRegExp("^(abstract|assert|break|case|catch|class|const|continue|default|do|else|enum|extends|false|final|finally|for|goto|if|implements|import|instanceof|interface|native|new|null|package|private|protected|public|return|static|strictfp|super|switch|synchronized|this|throw|throws|transient|true|try|volatile|while)(?![\\p{L}0-9_])");

JavaHighlighterDefinition.TYPES = BdmlXRegExp("^(boolean|byte|char|double|float|int|long|short|void)(?![\\p{L}0-9_])");

JavaHighlighterDefinition.prototype.PATTERNS = Object.freeze({
	STRING_ARRAY:            HighlighterPatterns.STRING_ARRAY
	, STRING_CONSTANT:       HighlighterPatterns.STRING_CONSTANT
	, NEWLINE:               HighlighterPatterns.NEWLINE
	, WHITESPACE:            HighlighterPatterns.WHITESPACE
	, COMMENT_BLOCK_START:   JavaHighlighterDefinition.COMMENT_BLOCK_START
	, COMMENT_LINE:          JavaHighlighterDefinition.COMMENT_LINE
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
	, KEYWORD:               JavaHighlighterDefinition.KEYWORDS
	, TYPE_NAME:             JavaHighlighterDefinition.TYPES
	, NAME:                  JavaHighlighterDefinition.NAME
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

JavaHighlighterDefinition.prototype.ELEMENT_NAMES = {};

JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.UNKNOWN] =                HighlighterElementNames.UNKNOWN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.STRING_ARRAY] =           HighlighterElementNames.STRING;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.STRING_CONSTANT] =        HighlighterElementNames.STRING;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.NEWLINE] =                HighlighterElementNames.EMPTY;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.WHITESPACE] =             HighlighterElementNames.EMPTY;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.APIDOC] =                 HighlighterElementNames.API_DOC;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_LINE] =           HighlighterElementNames.COMMENT;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_BLOCK_START] =    HighlighterElementNames.COMMENT;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_BLOCK_TEXT] =     HighlighterElementNames.COMMENT;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_BLOCK_END] =      HighlighterElementNames.COMMENT;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OPEN_SQUARE] =            HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CLOSE_SQUARE] =           HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OPEN_PARENTHESIS] =       HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CLOSE_PARENTHESIS] =      HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OPEN_CURLY] =             HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CLOSE_CURLY] =            HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.DOT] =                    HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SEMICOLON] =              HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMA] =                  HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COLON] =                  HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.LAMBDA] =                 HighlighterElementNames.LAMBDA;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.KEYWORD] =                HighlighterElementNames.KEYWORD;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.TYPE_NAME] =              HighlighterElementNames.TYPE_IDENTIFIER;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.NAME] =                   HighlighterElementNames.IDENTIFIER;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.NUMBER] =                 HighlighterElementNames.NUMBER;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.HEX_NUMBER] =             HighlighterElementNames.HEX_OCT_NUMBER;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OCTAL_NUMBER] =           HighlighterElementNames.HEX_OCT_NUMBER;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.BINARY_NUMBER] =          HighlighterElementNames.HEX_OCT_NUMBER;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_NUMBER] =            HighlighterElementNames.NUMBER;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_HEX_NUMBER] =        HighlighterElementNames.HEX_OCT_NUMBER;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_OCTAL_NUMBER] =      HighlighterElementNames.HEX_OCT_NUMBER;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_BINARY_NUMBER] =     HighlighterElementNames.HEX_OCT_NUMBER;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.QUESTION_MARK] =          HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.UNDERSCORE] =             HighlighterElementNames.UNDERSCORE;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EQUALS] =                 HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.LESS_THAN] =              HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.GREATER_THAN] =           HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.LESS_THAN_OR_EQUAL] =     HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.GREATER_THAN_OR_EQUAL] =  HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EXCLAMATION_EQUAL] =      HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EQUAL] =                  HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.AND] =                    HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OR] =                     HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.AND_AND] =                HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OR_OR] =                  HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.PLUS_EQUALS] =            HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MINUS_EQUALS] =           HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MULTIPLY_EQUALS] =        HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.DIVIDE_EQUALS] =          HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.AND_EQUALS] =             HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OR_EQUALS] =              HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.PLUS_PLUS] =              HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MINUS_MINUS] =            HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.PLUS] =                   HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MINUS] =                  HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EXCLAMATION] =            HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MULTIPLY] =               HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.DIVIDE] =                 HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_LEFT_EQUALS] =      HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_RIGHT3_EQUALS] =    HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_RIGHT_EQUALS] =     HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_RIGHT3] =           HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_RIGHT] =            HighlighterElementNames.PLAIN;
JavaHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.ANNOTATION] =             HighlighterElementNames.ANNOTATION;

Object.freeze(JavaHighlighterDefinition.prototype.ELEMENT_NAMES);

JavaHighlighterDefinition.prototype.getName = function () {
	return this.LANGUAGE_NAME;
};

JavaHighlighterDefinition.prototype.hasPrompts = function () {
	return false;
};

JavaHighlighterDefinition.prototype.prompt1 = function () {
	return null;
};

JavaHighlighterDefinition.prototype.prompt2 = function () {
	return null;
};

JavaHighlighterDefinition.prototype.getCommentBlockType = function () {
	return SimpleHighlighterDefinition.CommentBlockType.NON_RECURSIVE;
};

JavaHighlighterDefinition.prototype.commentBlockStart = function () {
	return JavaHighlighterDefinition.COMMENT_BLOCK_START;
};

JavaHighlighterDefinition.prototype.commentBlockEnd = function () {
	return JavaHighlighterDefinition.COMMENT_BLOCK_END;
};

JavaHighlighterDefinition.prototype.patterns = function () {
	return this.PATTERNS;
};

JavaHighlighterDefinition.prototype.elementNames = function () {
	return this.ELEMENT_NAMES;
};

JavaHighlighterDefinition.prototype.backslashIsLineContinuation = function () {
	return false;
};

JavaHighlighterDefinition.prototype.preHighlightingProcessing = function (input) {
	return input;
};