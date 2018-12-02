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

// TOC menu show/hide supporting Javascript.

var bdml_menu_hider_previous_onload = window.onload;

window.onload = function () {
	if (bdml_menu_hider_previous_onload) {
		bdml_menu_hider_previous_onload();
	}

	var tocTitle = document.getElementById("bdml-toc-title");
	tocTitle.className = "bdml-toc-title-hide";
	tocTitle.style.cursor = "pointer";

	tocTitle.onclick = function() {
		var tocUl = document.getElementById("bdml-toc-ul");

		if (tocUl.className === "bdml-hide-toc") {
			tocUl.className = "";
			tocTitle.className = "";
			tocTitle.className = "bdml-toc-title-hide";
		} else {
			tocUl.className = "bdml-hide-toc";
			tocTitle.className = "bdml-toc-title-show";
		}
	};
};
