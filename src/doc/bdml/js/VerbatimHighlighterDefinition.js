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

// Syntax highlighter definition for verbatim code.
// This does not highlight, but does remove common leading space.

function VerbatimHighlighterDefinition() {}

VerbatimHighlighterDefinition.prototype.LANGUAGE_NAME = "None";
VerbatimHighlighterDefinition.prototype.PATTERNS = Object.freeze({ PLAIN: BdmlXRegExp(/^.*/) });

VerbatimHighlighterDefinition.prototype.ELEMENT_NAMES = {};
VerbatimHighlighterDefinition.prototype.ELEMENT_NAMES[HighlighterToken._PLAIN_] = HighlighterElementNames.PLAIN;

Object.freeze(VerbatimHighlighterDefinition.prototype.ELEMENT_NAMES);

VerbatimHighlighterDefinition.prototype.getName = function () {
	return this.LANGUAGE_NAME;
};

VerbatimHighlighterDefinition.prototype.hasPrompts = function () {
	return false;
};

VerbatimHighlighterDefinition.prototype.prompt1 = function () {
	return null;
};

VerbatimHighlighterDefinition.prototype.prompt2 = function () {
	return null;
};

VerbatimHighlighterDefinition.prototype.getCommentBlockType = function () {
	return SimpleHighlighterDefinition.CommentBlockType.NONE;
};

VerbatimHighlighterDefinition.prototype.commentBlockStart = function () {
	return null;
};

VerbatimHighlighterDefinition.prototype.commentBlockEnd = function () {
	return null;
};

VerbatimHighlighterDefinition.prototype.patterns = function () {
	return this.PATTERNS;
};

VerbatimHighlighterDefinition.prototype.elementNames = function () {
	return this.ELEMENT_NAMES;
};

VerbatimHighlighterDefinition.prototype.backslashIsLineContinuation = function () {
	return false;
};

VerbatimHighlighterDefinition.prototype.preHighlightingProcessing = function (input) {
	return input;
};