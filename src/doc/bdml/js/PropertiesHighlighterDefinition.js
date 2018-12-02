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

// Syntax highlighter definition for hierarchical properties code.

function PropertiesHighlighterDefinition() {}

PropertiesHighlighterDefinition.prototype.LANGUAGE_NAME = "PROPERTIES";

PropertiesHighlighterDefinition.COMMENT_BLOCK_LINE = BdmlXRegExp("^\\s*(#|!).*$");

PropertiesHighlighterDefinition.INCLUDE = BdmlXRegExp("^\\s*@.*$");

PropertiesHighlighterDefinition.LINE_CONTINUATION = BdmlXRegExp("\\\\s*$");
PropertiesHighlighterDefinition.COMMENT_LINE_START = BdmlXRegExp("^\\s*([#!])");

PropertiesHighlighterDefinition.KEY_VALUE_BASE = "[^{}:= \t\n#!\\\\]+|\\{|\\}|\\\\:|\\\\=|\\#|\\!|(\\\\ |\t)|\\\\|\\\\[^{}:=#!\\\\ \\\\t\\\\r\\\\n]";
PropertiesHighlighterDefinition.LINE_CONT = "\\\\(\n|\n\n|\n|\n)( |\t)*";
PropertiesHighlighterDefinition.KEY_STRING = "(" + PropertiesHighlighterDefinition.KEY_VALUE_BASE + "|("+PropertiesHighlighterDefinition.LINE_CONT+"("+ PropertiesHighlighterDefinition.KEY_VALUE_BASE +"|#|!|"+PropertiesHighlighterDefinition.LINE_CONT+")*))+";
PropertiesHighlighterDefinition.KEY_VALUE_SEPARATOR = BdmlXRegExp("^\\s*(:|=| )\\s*");
PropertiesHighlighterDefinition.VALUE = BdmlXRegExp("^(" +PropertiesHighlighterDefinition.KEY_VALUE_BASE + "|#|!|}|:|=| |\t|("+PropertiesHighlighterDefinition.LINE_CONT+"("+PropertiesHighlighterDefinition.KEY_VALUE_BASE+"|#|!|}|:|=| |\t|\\{"+PropertiesHighlighterDefinition.LINE_CONT+")*))+");

PropertiesHighlighterDefinition.KEY = BdmlXRegExp(PropertiesHighlighterDefinition.KEY_STRING);

PropertiesHighlighterDefinition.COMMENT_LINE = BdmlXRegExp("^\/\/.*");

PropertiesHighlighterDefinition.NAME = new BdmlXRegExp("^[\\p{L}_][\\p{L}0-9_]*");

PropertiesHighlighterDefinition.KEYWORDS = BdmlXRegExp("^(abstract|assert|break|case|catch|class|const|continue|default|do|else|enum|extends|false|final|finally|for|goto|if|implements|import|instanceof|interface|native|new|null|package|private|protected|public|return|static|strictfp|super|switch|synchronized|this|throw|throws|transient|true|try|volatile|while)(?![\\p{L}0-9_])");

PropertiesHighlighterDefinition.TYPES = BdmlXRegExp("^(boolean|byte|char|double|float|int|long|short|void)(?![\\p{L}0-9_])");

PropertiesHighlighterDefinition.prototype.PATTERNS = Object.freeze({
	NEWLINE:                 HighlighterPatterns.NEWLINE
	, COMMENT_BLOCK_LINE:   PropertiesHighlighterDefinition.COMMENT_BLOCK_LINE
	, INCLUDE:               PropertiesHighlighterDefinition.INCLUDE
	, OPEN_CURLY:            HighlighterPatterns.OPEN_CURLY
	, CLOSE_CURLY:           HighlighterPatterns.CLOSE_CURLY
	, KEY_VALUE_SEPARATOR:   PropertiesHighlighterDefinition.KEY_VALUE_SEPARATOR
	, KEY:                   PropertiesHighlighterDefinition.KEY
	, VALUE:                 PropertiesHighlighterDefinition.VALUE
});

PropertiesHighlighterDefinition.prototype.ELEMENT_NAMES = {};

PropertiesHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.UNKNOWN] =                HighlighterElementNames.UNKNOWN;
PropertiesHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.NEWLINE] =                HighlighterElementNames.EMPTY;
PropertiesHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.COMMENT_BLOCK_LINE] =      HighlighterElementNames.COMMENT;
PropertiesHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.OPEN_CURLY] =             HighlighterElementNames.PLAIN;
PropertiesHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.CLOSE_CURLY] =            HighlighterElementNames.PLAIN;
PropertiesHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.KEY_VALUE_SEPARATOR] =    HighlighterElementNames.KEY_VALUE_SEPARATOR;
PropertiesHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.KEY] =                    HighlighterElementNames.KEY;
PropertiesHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.VALUE] =                  HighlighterElementNames.VALUE;
PropertiesHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken.INCLUDE] =                HighlighterElementNames.INCLUDE;

Object.freeze(PropertiesHighlighterDefinition.prototype.ELEMENT_NAMES);

PropertiesHighlighterDefinition.prototype.getName = function () {
	return this.LANGUAGE_NAME;
};

PropertiesHighlighterDefinition.prototype.hasPrompts = function () {
	return false;
};

PropertiesHighlighterDefinition.prototype.prompt1 = function () {
	return null;
};

PropertiesHighlighterDefinition.prototype.prompt2 = function () {
	return null;
};

PropertiesHighlighterDefinition.prototype.getCommentBlockType = function () {
	return SimpleHighlighterDefinition.CommentBlockType.NON_RECURSIVE;
};

PropertiesHighlighterDefinition.prototype.commentBlockStart = function () {
	return PropertiesHighlighterDefinition.COMMENT_BLOCK_START;
};

PropertiesHighlighterDefinition.prototype.commentBlockEnd = function () {
	return PropertiesHighlighterDefinition.COMMENT_BLOCK_END;
};

PropertiesHighlighterDefinition.prototype.patterns = function () {
	return this.PATTERNS;
};

PropertiesHighlighterDefinition.prototype.elementNames = function () {
	return this.ELEMENT_NAMES;
};

PropertiesHighlighterDefinition.prototype.backslashIsLineContinuation = function () {
	return false;
};

PropertiesHighlighterDefinition.prototype.preHighlightingProcessing = function (input) {
	var output = [];
	for (var i = 0; i < input.length; i++) {
		var line = input[i];
		var matcher = PropertiesHighlighterDefinition.LINE_CONTINUATION.exec(line);
		var currentLineCommentMatcher = PropertiesHighlighterDefinition.COMMENT_LINE_START.exec(line);
		var nextLineCommentMatcher = null;
		if(i<input.length -1 ) {
			nextLineCommentMatcher = PropertiesHighlighterDefinition.COMMENT_LINE_START.exec(input[i+1]);
		}
		while(matcher != null && ( currentLineCommentMatcher == null || nextLineCommentMatcher != null)) {
			line = line + "\n" + input[i+1];
			matcher = PropertiesHighlighterDefinition.LINE_CONTINUATION.exec(line);
			currentLineCommentMatcher = PropertiesHighlighterDefinition.COMMENT_LINE_START.exec(line);
			i++;
			if(i<input.length -1 ) {
				nextLineCommentMatcher = PropertiesHighlighterDefinition.COMMENT_LINE_START.exec(input[i+1]);
			} else {
				nextLineCommentMatcher = null;
			}
		}
		output.push(line);
	}
	return output;
};
