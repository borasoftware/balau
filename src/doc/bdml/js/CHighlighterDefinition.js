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

// Syntax highlighter definition for C code.

function CHighlighterDefinition() {}

CHighlighterDefinition.prototype.LANGUAGE_NAME = "C";
CHighlighterDefinition.COMMENT_BLOCK_START = BdmlXRegExp("^/\\*");
CHighlighterDefinition.COMMENT_BLOCK_END = BdmlXRegExp("\\*/");

CHighlighterDefinition.COMMENT_LINE = BdmlXRegExp("^\/\/.*");

CHighlighterDefinition.NAME = new BdmlXRegExp("^[\\p{L}_][\\p{L}0-9_]*");

CHighlighterDefinition.KEYWORDS = BdmlXRegExp("^(|auto|asm|break|case|const|continue|default|do|else|enum|extern|for|goto|if|inline|register|restrict|return|sizeof|static|struct|switch|typedef|union|volatile|while )(?![\\p{L}0-9_])");

CHighlighterDefinition.TYPES = BdmlXRegExp("^(_Bool|char|_Complex|double|float|_Imaginary|int|long|short|signed|unsigned|void)(?![\\p{L}0-9_])");

CHighlighterDefinition.prototype.PATTERNS = Object.freeze({
	STRING_ARRAY:            HighlighterPatterns.STRING_ARRAY
	, STRING_CONSTANT:       HighlighterPatterns.STRING_CONSTANT
	, NEWLINE:               HighlighterPatterns.NEWLINE
	, WHITESPACE:            HighlighterPatterns.WHITESPACE
	, CPP_DEFINE:            HighlighterPatterns.CPP_DEFINE
	, COMMENT_BLOCK_START:   CHighlighterDefinition.COMMENT_BLOCK_START
	, COMMENT_LINE:          CHighlighterDefinition.COMMENT_LINE
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
	, KEYWORD:               CHighlighterDefinition.KEYWORDS
	, TYPE_NAME:             CHighlighterDefinition.TYPES
	, NAME:                  CHighlighterDefinition.NAME
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

CHighlighterDefinition.prototype.ELEMENT_NAMES = {};

CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.UNKNOWN] =                HighlighterElementNames.UNKNOWN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.STRING_ARRAY] =           HighlighterElementNames.STRING;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.STRING_CONSTANT] =        HighlighterElementNames.STRING;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.NEWLINE] =                HighlighterElementNames.EMPTY;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.WHITESPACE] =             HighlighterElementNames.EMPTY;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.APIDOC] =                 HighlighterElementNames.API_DOC;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_LINE] =           HighlighterElementNames.COMMENT;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_BLOCK_START] =    HighlighterElementNames.COMMENT;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_BLOCK_TEXT] =     HighlighterElementNames.COMMENT;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_BLOCK_END] =      HighlighterElementNames.COMMENT;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CPP_DEFINE] =             HighlighterElementNames.CPP_DEFINE;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OPEN_SQUARE] =            HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CLOSE_SQUARE] =           HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OPEN_PARENTHESIS] =       HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CLOSE_PARENTHESIS] =      HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OPEN_CURLY] =             HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CLOSE_CURLY] =            HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.DOT] =                    HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SEMICOLON] =              HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMA] =                  HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COLON] =                  HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.LAMBDA] =                 HighlighterElementNames.LAMBDA;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.KEYWORD] =                HighlighterElementNames.KEYWORD;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.TYPE_NAME] =              HighlighterElementNames.TYPE_IDENTIFIER;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.NAME] =                   HighlighterElementNames.IDENTIFIER;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.NUMBER] =                 HighlighterElementNames.NUMBER;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.HEX_NUMBER] =             HighlighterElementNames.HEX_OCT_NUMBER;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OCTAL_NUMBER] =           HighlighterElementNames.HEX_OCT_NUMBER;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.BINARY_NUMBER] =          HighlighterElementNames.HEX_OCT_NUMBER;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_NUMBER] =            HighlighterElementNames.NUMBER;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_HEX_NUMBER] =        HighlighterElementNames.HEX_OCT_NUMBER;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_OCTAL_NUMBER] =      HighlighterElementNames.HEX_OCT_NUMBER;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.IMAG_BINARY_NUMBER] =     HighlighterElementNames.HEX_OCT_NUMBER;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.QUESTION_MARK] =          HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EQUALS] =                 HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.LESS_THAN] =              HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.GREATER_THAN] =           HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.LESS_THAN_OR_EQUAL] =     HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.GREATER_THAN_OR_EQUAL] =  HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EXCLAMATION_EQUAL] =      HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EQUAL] =                  HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.AND] =                    HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OR] =                     HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.AND_AND] =                HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OR_OR] =                  HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.PLUS_EQUALS] =            HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MINUS_EQUALS] =           HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MULTIPLY_EQUALS] =        HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.DIVIDE_EQUALS] =          HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.AND_EQUALS] =             HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OR_EQUALS] =              HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.PLUS_PLUS] =              HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MINUS_MINUS] =            HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.PLUS] =                   HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MINUS] =                  HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.EXCLAMATION] =            HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.MULTIPLY] =               HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.DIVIDE] =                 HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.HAT] =                    HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.DOUBLE_HASH] =            HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.BACKSLASH] =              HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_LEFT_EQUALS] =      HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_RIGHT_EQUALS] =     HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_LEFT] =             HighlighterElementNames.PLAIN;
CHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.SHIFT_RIGHT] =            HighlighterElementNames.PLAIN;

Object.freeze(CHighlighterDefinition.prototype.ELEMENT_NAMES);

CHighlighterDefinition.prototype.getName = function () {
	return this.LANGUAGE_NAME;
};

CHighlighterDefinition.prototype.hasPrompts = function () {
	return false;
};

CHighlighterDefinition.prototype.prompt1 = function () {
	return null;
};

CHighlighterDefinition.prototype.prompt2 = function () {
	return null;
};

CHighlighterDefinition.prototype.getCommentBlockType = function () {
	return SimpleHighlighterDefinition.CommentBlockType.NON_RECURSIVE;
};

CHighlighterDefinition.prototype.commentBlockStart = function () {
	return CHighlighterDefinition.COMMENT_BLOCK_START;
};

CHighlighterDefinition.prototype.commentBlockEnd = function () {
	return CHighlighterDefinition.COMMENT_BLOCK_END;
};

CHighlighterDefinition.prototype.patterns = function () {
	return this.PATTERNS;
};

CHighlighterDefinition.prototype.elementNames = function () {
	return this.ELEMENT_NAMES;
};

CHighlighterDefinition.prototype.backslashIsLineContinuation = function () {
	return false; // TODO WIP
};

CHighlighterDefinition.prototype.preHighlightingProcessing = function (input) {
	return input;
};
