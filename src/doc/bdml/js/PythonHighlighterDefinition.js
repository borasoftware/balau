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

// Syntax highlighter definition for Python code.

function PythonHighlighterDefinition() {}

PythonHighlighterDefinition.prototype.LANGUAGE_NAME = "PYTHON";

PythonHighlighterDefinition.COMMENT_BLOCK_START = BdmlXRegExp("^/\\*");
PythonHighlighterDefinition.COMMENT_BLOCK_END = BdmlXRegExp("\\*/");

PythonHighlighterDefinition.COMMENT_LINE = BdmlXRegExp("^\/\/.*");

PythonHighlighterDefinition.NAME = new BdmlXRegExp("^[\\p{L}_][\\p{L}0-9_]*");

PythonHighlighterDefinition.KEYWORDS = BdmlXRegExp("^(abstract|assert|break|case|catch|class|const|continue|default|do|else|enum|extends|false|final|finally|for|goto|if|implements|import|instanceof|interface|native|new|null|package|private|protected|public|return|static|strictfp|super|switch|synchronized|this|throw|throws|transient|true|try|volatile|while)(?![\\p{L}0-9_])");

PythonHighlighterDefinition.TYPES = BdmlXRegExp("^(boolean|byte|char|double|float|int|long|short|void)(?![\\p{L}0-9_])");

PythonHighlighterDefinition.prototype.PATTERNS = Object.freeze({
	STRING_ARRAY:            HighlighterPatterns.STRING_ARRAY
	, STRING_CONSTANT:       HighlighterPatterns.STRING_CONSTANT
	, NEWLINE:               HighlighterPatterns.NEWLINE
	, WHITESPACE:            HighlighterPatterns.WHITESPACE
	, CPP_DEFINE:            HighlighterPatterns.CPP_DEFINE
	, COMMENT_BLOCK_START:   PythonHighlighterDefinition.COMMENT_BLOCK_START
	, COMMENT_LINE:          PythonHighlighterDefinition.COMMENT_LINE
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
	, KEYWORD:               PythonHighlighterDefinition.KEYWORDS
	, NAME:                  PythonHighlighterDefinition.NAME
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
	, HAT:                   HighlighterPatterns.HAT
	, BACKSLASH:             HighlighterPatterns.BACKSLASH
	, DOUBLE_HASH:           HighlighterPatterns.DOUBLE_HASH
	, AND_AND:               HighlighterPatterns.AND_AND
	, OR_OR:                 HighlighterPatterns.OR_OR
	, AND:                   HighlighterPatterns.AND
	, OR:                    HighlighterPatterns.OR
	, QUESTION_MARK:         HighlighterPatterns.QUESTION_MARK
	, SHIFT_LEFT_EQUALS:     HighlighterPatterns.SHIFT_LEFT_EQUALS
	, SHIFT_RIGHT_EQUALS:    HighlighterPatterns.SHIFT_RIGHT_EQUALS
	, SHIFT_LEFT:            HighlighterPatterns.SHIFT_LEFT
	, SHIFT_RIGHT:           HighlighterPatterns.SHIFT_RIGHT
});

PythonHighlighterDefinition.prototype.ELEMENT_NAMES = {};

PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.UNKNOWN] =                HighlighterElementNames.UNKNOWN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.STRING_ARRAY] =           HighlighterElementNames.STRING;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.STRING_CONSTANT] =        HighlighterElementNames.STRING;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.NEWLINE] =                HighlighterElementNames.EMPTY;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.WHITESPACE] =             HighlighterElementNames.EMPTY;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.APIDOC] =                 HighlighterElementNames.API_DOC;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_LINE] =           HighlighterElementNames.COMMENT;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_BLOCK_START] =    HighlighterElementNames.COMMENT;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_BLOCK_TEXT] =     HighlighterElementNames.COMMENT;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_BLOCK_END] =      HighlighterElementNames.COMMENT;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CPP_DEFINE] =             HighlighterElementNames.CPP_DEFINE;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OPEN_SQUARE] =            HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CLOSE_SQUARE] =           HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OPEN_PARENTHESIS] =       HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CLOSE_PARENTHESIS] =      HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OPEN_CURLY] =             HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CLOSE_CURLY] =            HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.DOT] =                    HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SEMICOLON] =              HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMA] =                  HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COLON] =                  HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.LAMBDA] =                 HighlighterElementNames.LAMBDA;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.KEYWORD] =                HighlighterElementNames.KEYWORD;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.NAME] =                   HighlighterElementNames.IDENTIFIER;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.NUMBER] =                 HighlighterElementNames.NUMBER;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.HEX_NUMBER] =             HighlighterElementNames.HEX_OCT_NUMBER;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OCTAL_NUMBER] =           HighlighterElementNames.HEX_OCT_NUMBER;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.BINARY_NUMBER] =          HighlighterElementNames.HEX_OCT_NUMBER;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_NUMBER] =            HighlighterElementNames.NUMBER;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_HEX_NUMBER] =        HighlighterElementNames.HEX_OCT_NUMBER;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_OCTAL_NUMBER] =      HighlighterElementNames.HEX_OCT_NUMBER;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_BINARY_NUMBER] =     HighlighterElementNames.HEX_OCT_NUMBER;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.QUESTION_MARK] =          HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EQUALS] =                 HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.LESS_THAN] =              HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.GREATER_THAN] =           HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.LESS_THAN_OR_EQUAL] =     HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.GREATER_THAN_OR_EQUAL] =  HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EXCLAMATION_EQUAL] =      HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EQUAL] =                  HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.AND] =                    HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OR] =                     HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.AND_AND] =                HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OR_OR] =                  HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.PLUS_EQUALS] =            HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MINUS_EQUALS] =           HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MULTIPLY_EQUALS] =        HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.DIVIDE_EQUALS] =          HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.AND_EQUALS] =             HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OR_EQUALS] =              HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.PLUS_PLUS] =              HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MINUS_MINUS] =            HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.PLUS] =                   HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MINUS] =                  HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EXCLAMATION] =            HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MULTIPLY] =               HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.DIVIDE] =                 HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.HAT] =                    HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.DOUBLE_HASH] =            HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.BACKSLASH] =              HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_LEFT_EQUALS] =      HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_RIGHT_EQUALS] =     HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_LEFT] =             HighlighterElementNames.PLAIN;
PythonHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_RIGHT] =            HighlighterElementNames.PLAIN;

Object.freeze(PythonHighlighterDefinition.prototype.ELEMENT_NAMES);

PythonHighlighterDefinition.prototype.getName = function () {
	return this.LANGUAGE_NAME;
};

PythonHighlighterDefinition.prototype.hasPrompts = function () {
	return false;
};

PythonHighlighterDefinition.prototype.prompt1 = function () {
	return null;
};

PythonHighlighterDefinition.prototype.prompt2 = function () {
	return null;
};

PythonHighlighterDefinition.prototype.getCommentBlockType = function () {
	return SimpleHighlighterDefinition.CommentBlockType.NON_RECURSIVE;
};

PythonHighlighterDefinition.prototype.commentBlockStart = function () {
	return PythonHighlighterDefinition.COMMENT_BLOCK_START;
};

PythonHighlighterDefinition.prototype.commentBlockEnd = function () {
	return PythonHighlighterDefinition.COMMENT_BLOCK_END;
};

PythonHighlighterDefinition.prototype.patterns = function () {
	return this.PATTERNS;
};

PythonHighlighterDefinition.prototype.elementNames = function () {
	return this.ELEMENT_NAMES;
};

PythonHighlighterDefinition.prototype.backslashIsLineContinuation = function () {
	return false; // TODO WIP
};

PythonHighlighterDefinition.prototype.preHighlightingProcessing = function (input) {
	return input;
};