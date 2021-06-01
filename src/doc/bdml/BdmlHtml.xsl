<?xml version="1.0" encoding="utf-8"?>

<!--
  -
  - BDML -> HTML XSLT 1.0 transform.
  -
  - Copyright 2007 Bora Software (contact@borasoftware.com)
  - https://borasoftware.com/specifications/bdml.html
  -
  - This program is free software: you can redistribute it and/or modify
  - it under the terms of the GNU Affero General Public License as published
  - by the Free Software Foundation, either version 3 of the License, or
  - (at your option) any later version.
  -
  - This program is distributed in the hope that it will be useful,
  - but WITHOUT ANY WARRANTY; without even the implied warranty of
  - MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  - GNU Affero General Public License for more details.
  -
  - You should have received a copy of the GNU Affero General Public License
  - along with this program. If not, see https://www.gnu.org/licenses/.
  -
 -->

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
               xmlns:exsl="http://exslt.org/common"
               xmlns:bd="http://boradoc.org/1.0"
               exclude-result-prefixes="bd"
               version="1.0">
	<xsl:output method="html" doctype-system="about:legacy-compat" />
	<xsl:strip-space elements="*" />
	<xsl:preserve-space elements="text pre bd:code bd:xml bd:para bd:h1 bd:h2 bd:h3 bd:h4 bd:h5 bd:grammar" />

	<!-- Set this to false if converting to HTML via a standard XSLT processor. -->
	<xsl:param name="no-link-conversion" select="'true'" />

	<xsl:variable name="lowercase" select="'abcdefghijklmnopqrstuvwxyz'" />
	<xsl:variable name="uppercase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'" />
	<xsl:variable name="bdmlFileExtension" select="'.bdml'" />
	<xsl:variable name="target" select="'html'" />
	<xsl:variable name="bdmlPath" select="''" />
	<xsl:variable name="variant" select="''" />
	<xsl:variable name="newline"><xsl:text>&#xa;</xsl:text></xsl:variable>

	<xsl:variable name="relativeRoot">
		<xsl:if test="count(/bd:document/bd:metadata/bd:relative-root) > 1">
			<xsl:call-template name="reportError">
				<xsl:with-param name="message" select="'Multiple relative-root metadata elements were found.'" />
			</xsl:call-template>
		</xsl:if>

		<xsl:choose>
			<xsl:when test="/bd:document/bd:metadata/bd:relative-root">
				<xsl:variable name="url" select="/bd:document/bd:metadata/bd:relative-root/@url" />

				<xsl:choose>
					<xsl:when test="not($url)">
						<xsl:message terminate="yes">Error: the relative-root metadata element does not have a 'url' attribute.</xsl:message>
					</xsl:when>

					<xsl:otherwise>
						<xsl:choose>
							<xsl:when test="substring($url, string-length($url)) = '/'">
								<xsl:value-of select="$url" />
							</xsl:when>

							<xsl:otherwise>
								<xsl:value-of select="concat($url, '/')" />
							</xsl:otherwise>
						</xsl:choose>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>

			<xsl:otherwise>
				<xsl:value-of select="''" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<xsl:variable name="tocStart">
		<xsl:variable name="qualifying-tocs">
			<xsl:call-template name="applicable-targets-and-variants">
				<xsl:with-param name="elements" select="/bd:document/bd:metadata/bd:toc" />
				<xsl:with-param name="theTarget" select="$target" />
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="count(exsl:node-set($qualifying-tocs)/*) > 1">
			<xsl:call-template name="reportError">
				<xsl:with-param name="message" select="concat('Multiple (', count(exsl:node-set($qualifying-tocs)/*), ') toc metadata elements were found to be applicable.')" />
			</xsl:call-template>
		</xsl:if>

		<xsl:choose>
			<xsl:when test="exsl:node-set($qualifying-tocs)/bd:toc">
				<xsl:variable name="tocElementMetadata" select="exsl:node-set($qualifying-tocs)/bd:toc" />

				<xsl:choose>
					<xsl:when test="$tocElementMetadata and $tocElementMetadata/@start">
						<xsl:value-of select="$tocElementMetadata/@start" />
					</xsl:when>

					<xsl:otherwise>
						<xsl:value-of select="0" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>

			<xsl:otherwise>
				<xsl:value-of select="99" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<xsl:variable name="tocFinish">
		<xsl:variable name="qualifying-tocs">
			<xsl:call-template name="applicable-targets-and-variants">
				<xsl:with-param name="elements" select="/bd:document/bd:metadata/bd:toc" />
				<xsl:with-param name="theTarget" select="$target" />
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="count(exsl:node-set($qualifying-tocs)/*) > 1">
			<xsl:call-template name="reportError">
				<xsl:with-param name="message" select="concat('Multiple (', count(exsl:node-set($qualifying-tocs)/*), ') toc metadata elements were found to be applicable.')" />
			</xsl:call-template>
		</xsl:if>

		<xsl:choose>
			<xsl:when test="exsl:node-set($qualifying-tocs)/bd:toc">
				<xsl:variable name="tocElementMetadata" select="exsl:node-set($qualifying-tocs)/bd:toc" />

				<xsl:choose>
					<xsl:when test="$tocElementMetadata and $tocElementMetadata/@finish">
						<xsl:value-of select="$tocElementMetadata/@finish" />
					</xsl:when>

					<xsl:otherwise>
						<xsl:value-of select="5" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>

			<xsl:otherwise>
				<xsl:value-of select="-99" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<xsl:variable name="tocPlaceholder" select="/bd:document/bd:chapter[@id='bdml-toc']" />

	<xsl:variable name="frontMatterCount">
		<xsl:choose>
			<xsl:when test="/bd:document/bd:part[1]">
				<xsl:value-of select="count(/bd:document/bd:part[1]/preceding-sibling::bd:chapter)" />
			</xsl:when>

			<xsl:otherwise>
				<xsl:value-of select="count(/bd:document/bd:chapter[not(contains(@class, 'bdml-front-matter') or contains(@class, 'bdml-header') or contains(@class, 'bdml-footer'))][1]/preceding-sibling::bd:chapter)" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<xsl:variable name="nonAppendixCount">
		<xsl:variable name="non-appendix-count">
			<xsl:call-template name="getChapterElements" />
		</xsl:variable>

		<xsl:value-of select="count(exsl:node-set($non-appendix-count)/bd:chapter[not(@type = 'appendix') and not(@type = 'APPENDIX')])" />
	</xsl:variable>

	<xsl:variable name="numberingStart">
		<xsl:variable name="qualifying-numberings">
			<xsl:call-template name="applicable-targets-and-variants">
				<xsl:with-param name="elements" select="/bd:document/bd:metadata/bd:numbering" />
				<xsl:with-param name="theTarget" select="$target" />
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="count(exsl:node-set($qualifying-numberings)/*) > 1">
			<xsl:call-template name="reportError">
				<xsl:with-param name="message" select="concat('Multiple (', count(exsl:node-set($qualifying-numberings)/*), ') numbering metadata elements were found to be applicable.')" />
			</xsl:call-template>
		</xsl:if>

		<xsl:choose>
			<xsl:when test="exsl:node-set($qualifying-numberings)/bd:numbering">
				<xsl:variable name="numberingElementMetadata" select="exsl:node-set($qualifying-numberings)/bd:numbering" />
				<xsl:variable name="chapters" select="normalize-space($numberingElementMetadata/@chapters)" />
				<xsl:variable name="finish" select="normalize-space($numberingElementMetadata/@finish)" />

				<xsl:variable name="start">
					<xsl:choose>
						<xsl:when test="string-length($chapters) > 0">
							<xsl:choose>
								<xsl:when test="translate($chapters, $uppercase, $lowercase) = 'true'">
									<xsl:value-of select="0" />
								</xsl:when>

								<xsl:otherwise>
									<xsl:value-of select="1" />
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>

						<xsl:otherwise>
							<xsl:value-of select="0" />
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>

				<xsl:choose>
					<xsl:when test="string-length($start) > 0">
						<xsl:choose>
							<xsl:when test="string-length($finish) > 0">
								<xsl:choose>
									<xsl:when test="$finish &lt; $start">
										<xsl:value-of select="99" />
									</xsl:when>

									<xsl:otherwise>
										<xsl:value-of select="$start" />
									</xsl:otherwise>
								</xsl:choose>
							</xsl:when>

							<xsl:otherwise>
								<xsl:value-of select="$start" />
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>

					<xsl:otherwise>
						<xsl:value-of select="-99" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>

			<xsl:otherwise>
				<xsl:value-of select="99" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<xsl:variable name="numberingFinish">
		<xsl:variable name="qualifying-numberings">
			<xsl:call-template name="applicable-targets-and-variants">
				<xsl:with-param name="elements" select="/bd:document/bd:metadata/bd:numbering" />
				<xsl:with-param name="theTarget" select="$target" />
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="count(exsl:node-set($qualifying-numberings)/*) > 1">
			<xsl:call-template name="reportError">
				<xsl:with-param name="message" select="concat('Multiple (', count(exsl:node-set($qualifying-numberings)/*), ') numbering metadata elements were found to be applicable.')" />
			</xsl:call-template>
		</xsl:if>

		<xsl:choose>
			<xsl:when test="exsl:node-set($qualifying-numberings)/bd:numbering">
				<xsl:variable name="numberingElementMetadata" select="exsl:node-set($qualifying-numberings)/bd:numbering" />
				<xsl:variable name="chapters" select="normalize-space($numberingElementMetadata/@chapters)" />
				<xsl:variable name="finish" select="normalize-space($numberingElementMetadata/@finish)" />

				<xsl:variable name="start">
					<xsl:choose>
						<xsl:when test="string-length($chapters) > 0">
							<xsl:choose>
								<xsl:when test="translate($chapters, $uppercase, $lowercase) = 'true'">
									<xsl:value-of select="0" />
								</xsl:when>

								<xsl:otherwise>
									<xsl:value-of select="1" />
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>

						<xsl:otherwise>
							<xsl:value-of select="0" />
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>

				<xsl:choose>
					<xsl:when test="string-length($finish) > 0">
						<xsl:choose>
							<xsl:when test="string-length($start) > 0">
								<xsl:choose>
									<xsl:when test="$finish &lt; $start">
										<xsl:value-of select="-99" />
									</xsl:when>

									<xsl:otherwise>
										<xsl:value-of select="$finish" />
									</xsl:otherwise>
								</xsl:choose>
							</xsl:when>

							<xsl:otherwise>
								<xsl:value-of select="$finish" />
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>

					<xsl:otherwise>
						<xsl:value-of select="99" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>

			<xsl:otherwise>
				<xsl:value-of select="-99" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<xsl:variable name="chapterStartOffset">
		<xsl:variable name="qualifying-numberings">
			<xsl:call-template name="applicable-targets-and-variants">
				<xsl:with-param name="elements" select="/bd:document/bd:metadata/bd:numbering" />
				<xsl:with-param name="theTarget" select="$target" />
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="count(exsl:node-set($qualifying-numberings)/*) > 1">
			<xsl:call-template name="reportError">
				<xsl:with-param name="message" select="concat('Multiple (', count(exsl:node-set($qualifying-numberings)/*), ') numbering metadata elements were found to be applicable.')" />
			</xsl:call-template>
		</xsl:if>

		<xsl:choose>
			<xsl:when test="exsl:node-set($qualifying-numberings)/bd:numbering">
				<xsl:variable name="numberingElementMetadata" select="exsl:node-set($qualifying-numberings)/bd:numbering" />
				<xsl:variable name="root" select="normalize-space($numberingElementMetadata/@root)" />

				<xsl:choose>
					<xsl:when test="string-length($root) > 0">
						<xsl:value-of select="$root - 1" />
					</xsl:when>

					<xsl:otherwise>
						<xsl:value-of select="0" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>

			<xsl:otherwise>
				<xsl:value-of select="0" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:variable>

	<xsl:variable name="documentType" select="count(/bd:document/bd:part)" />

	<!-- ************************** ROOT ************************** -->

	<xsl:template match="/bd:document">
		<xsl:if test="bd:metadata/bd:copyright">
			<xsl:comment><xsl:value-of select="bd:metadata/bd:copyright" /></xsl:comment>
			<xsl:value-of select="$newline" />
			<xsl:comment> !! This file is automatically generated. Any manual changes will be lost on regeneration. !! </xsl:comment>
		</xsl:if>

		<xsl:value-of select="$newline" />

		<html>
			<head>
				<title>
					<xsl:choose>
						<xsl:when test="bd:metadata/bd:title">
							<xsl:value-of select="bd:metadata/bd:title/@text" />
						</xsl:when>

						<xsl:when test="bd:part/@title">
							<xsl:value-of select="(bd:part/@title)[1]" />
						</xsl:when>

						<xsl:when test="bd:part/bd:chapter/@title">
							<xsl:value-of select="(bd:part/bd:chapter/@title)[1]" />
						</xsl:when>

						<xsl:when test="bd:chapter/@title">
							<xsl:value-of select="(bd:chapter/@title)[1]" />
						</xsl:when>

						<xsl:otherwise>
							<xsl:value-of select="''" />
						</xsl:otherwise>
					</xsl:choose>
				</title>

				<xsl:for-each select="bd:metadata/bd:stylesheet[(not(@target) or @target = '' or contains(@target, $target)) and (not(@variant) or @variant = '' or contains(@variant, $variant))]
				| bd:metadata/bd:css[(not(@target) or @target = '' or contains(@target, $target)) and (not(@variant) or @variant = '' or contains(@variant, $variant))]">
					<xsl:choose>
						<xsl:when test="name() = 'stylesheet'">
							<link rel="stylesheet" type="text/css">
								<xsl:attribute name="href"><xsl:value-of select="concat($relativeRoot, @url)" /></xsl:attribute>
							</link>
						</xsl:when>
						<xsl:when test="name() = 'css'">
							<style>
								<xsl:value-of select="current()"/>
							</style>
						</xsl:when>
					</xsl:choose>

				</xsl:for-each>

				<xsl:if test="$target = 'html'">
					<!-- Automatically added meta elements. -->
					<meta http-equiv="Content-Type" content="text/html" charset="utf-8" />
					<meta name="viewport" content="width=device-width, initial-scale=1.0" />

					<!-- Explicitly added HTML meta elements in the main BDML file. -->
					<xsl:for-each select="bd:metadata/bd:meta[(not(@target) or @target = '' or contains(@target, 'html')) and (not(@variant) or @variant = '' or contains(@variant, $variant))]">
						<meta><xsl:apply-templates select="@* | node()" mode="verbatim" /></meta>
					</xsl:for-each>

					<!-- HTML link elements. -->
					<xsl:for-each select="bd:metadata/bd:link[(not(@target) or @target = '' or contains(@target, 'html')) and (not(@variant) or @variant = '' or contains(@variant, $variant))]">
						<link>
							<xsl:apply-templates select="@* | node()" mode="verbatim" />
							<xsl:attribute name="href"><xsl:value-of select="concat($relativeRoot, @href)" /></xsl:attribute>
						</link>
					</xsl:for-each>

					<!-- HTML script elements. -->
					<xsl:for-each select="bd:metadata/bd:script[not(@target) or @target = 'html']">
						<script>
							<xsl:for-each select="@*">
								<xsl:choose>
									<xsl:when test="name(.) = 'src'">
										<xsl:attribute name="src">
											<xsl:value-of select="concat($relativeRoot, .)" />
										</xsl:attribute>
									</xsl:when>

									<xsl:otherwise>
										<xsl:apply-templates select="." mode="verbatim" />
									</xsl:otherwise>
								</xsl:choose>
							</xsl:for-each>
						</script>
					</xsl:for-each>
				</xsl:if>
			</head>

			<xsl:value-of select="$newline" />

			<body>
				<div id="bdml-page">
					<!-- Optional header. -->
					<xsl:variable name="qualifying-headers">
						<xsl:call-template name="applicable-targets-and-variants">
							<xsl:with-param name="elements" select="bd:metadata/bd:header" />
							<xsl:with-param name="theTarget" select="'html'" />
						</xsl:call-template>
					</xsl:variable>

					<xsl:if test="count(exsl:node-set($qualifying-headers)/bd:header) > 1">
						<xsl:call-template name="reportError">
							<xsl:with-param name="message" select="'Multiple header metadata elements were found to be applicable.'" />
						</xsl:call-template>
					</xsl:if>

					<xsl:if test="exsl:node-set($qualifying-headers)/bd:header">
						<xsl:variable name="html-header" select="exsl:node-set($qualifying-headers)/bd:header" />

						<div id="bdml-header">
							<xsl:apply-templates select="document(concat($relativeRoot, $html-header/@url), .)/bd:document/bd:chapter/*" />
						</div>

						<xsl:value-of select="$newline" />
					</xsl:if>

					<!-- Optional cover graphic. -->
					<xsl:variable name="qualifying-covers">
						<xsl:call-template name="applicable-targets-and-variants">
							<xsl:with-param name="elements" select="bd:metadata/bd:cover" />
							<xsl:with-param name="theTarget" select="'html'" />
						</xsl:call-template>
					</xsl:variable>

					<xsl:if test="count(exsl:node-set($qualifying-covers)/bd:cover) > 1">
						<xsl:call-template name="reportError">
							<xsl:with-param name="message" select="'Multiple cover metadata elements were found to be applicable.'" />
						</xsl:call-template>
					</xsl:if>

					<xsl:if test="exsl:node-set($qualifying-covers)/bd:cover">
						<xsl:variable name="html-cover" select="exsl:node-set($qualifying-covers)/bd:cover" />
						<div class="bdml-cover"><xsl:apply-templates select="$html-cover" /></div>
						<xsl:value-of select="$newline" />
					</xsl:if>

					<!-- Optional TOC. -->
					<xsl:if test="$tocFinish > $tocStart">
						<div id="bdml-toc">
							<xsl:call-template name="buildToc" />
						</div>
					</xsl:if>

					<!-- Parts or chapters. -->
					<xsl:choose>
						<xsl:when test="bd:part">
							<xsl:apply-templates select="bd:part | bd:chapter | bd:include" />
						</xsl:when>

						<xsl:otherwise>
							<xsl:apply-templates select="bd:chapter | bd:include" />
						</xsl:otherwise>
					</xsl:choose>

					<xsl:value-of select="$newline" />

					<!-- Optional footer. -->
					<xsl:variable name="qualifying-footers">
						<xsl:call-template name="applicable-targets-and-variants">
							<xsl:with-param name="elements" select="bd:metadata/bd:footer" />
							<xsl:with-param name="theTarget" select="'html'" />
						</xsl:call-template>
					</xsl:variable>

					<xsl:if test="count(exsl:node-set($qualifying-footers)/bd:footer) > 1">
						<xsl:call-template name="reportError">
							<xsl:with-param name="message" select="'Multiple footer metadata elements were found to be applicable.'" />
						</xsl:call-template>
					</xsl:if>

					<xsl:if test="exsl:node-set($qualifying-footers)/bd:footer">
						<xsl:variable name="html-footer" select="exsl:node-set($qualifying-footers)/bd:footer" />

						<div id="bdml-footer">
							<xsl:apply-templates select="document(concat($relativeRoot, $html-footer/@url), .)/bd:document/bd:chapter/*" />
						</div>

						<xsl:value-of select="$newline" />
					</xsl:if>

					<xsl:value-of select="$newline" />
				</div>

				<xsl:value-of select="$newline" />
			</body>

			<xsl:value-of select="$newline" />
		</html>

		<xsl:value-of select="$newline" />
	</xsl:template>

	<!-- **************** NON-APPENDIX COUNT MODE ***************** -->

	<xsl:template name="getChapterElements">
		<xsl:variable name="chapter-elements">
			<!-- Parts/chapters or chapters. -->
			<xsl:choose>
				<xsl:when test="/bd:document/bd:part">
					<xsl:apply-templates select="/bd:document/bd:part | /bd:document/bd:chapter | /bd:document/bd:include" mode="CHAPTER-COUNT" />
				</xsl:when>

				<xsl:otherwise>
					<xsl:apply-templates select="/bd:document/bd:chapter | /bd:document/bd:include" mode="CHAPTER-COUNT" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:copy-of select="exsl:node-set($chapter-elements)" />
	</xsl:template>

	<xsl:template match="bd:part" mode="CHAPTER-COUNT">
		<xsl:variable name="chapter-elements">
			<xsl:apply-templates select="bd:chapter | bd:include" mode="CHAPTER-COUNT" />
		</xsl:variable>

		<xsl:copy-of select="exsl:node-set($chapter-elements)/*" />
	</xsl:template>

	<xsl:template match="bd:include" mode="CHAPTER-COUNT">

		<xsl:variable name="applies">
			<xsl:call-template name="target-and-variant-applies">
				<xsl:with-param name="target-list" select="@target" />
				<xsl:with-param name="variant-list" select="@variant" />
				<xsl:with-param name="theTarget" select="'html'" />
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="string($applies) = 'true'">
			<xsl:variable name="doc" select="document(concat($relativeRoot, @url), .)" />

			<xsl:if test="not($doc)">
				<xsl:call-template name="reportError">
					<xsl:with-param name="message" select="concat('the included document at ', $relativeRoot, @url, ' was not found.')" />
				</xsl:call-template>
			</xsl:if>

			<xsl:variable name="documentElement" select="$doc/bd:document" />

			<xsl:if test="not($documentElement)">
				<xsl:call-template name="reportError">
					<xsl:with-param name="message" select="concat('the document element of the included document at ', $relativeRoot, @url, ' was not found.')" />
				</xsl:call-template>
			</xsl:if>

			<xsl:variable name="chapter-elements">
				<xsl:apply-templates select="$documentElement/bd:chapter" mode="CHAPTER-COUNT" />
			</xsl:variable>

			<xsl:copy-of select="$chapter-elements" />
		</xsl:if>
	</xsl:template>

	<xsl:template match="bd:chapter" mode="CHAPTER-COUNT">
		<xsl:copy-of select="." />
	</xsl:template>

	<!-- ************************* TOC MODE *********************** -->

	<xsl:template name="buildToc">
		<div id="bdml-toc-title">Contents</div>
		<xsl:value-of select="$newline" />

		<ul id="bdml-toc-ul">
			<!-- Parts/chapters or chapters. -->
			<xsl:choose>
				<xsl:when test="/bd:document/bd:part">
					<xsl:choose>
						<xsl:when test="$tocPlaceholder">
							<xsl:apply-templates select="$tocPlaceholder/following-sibling::bd:part | $tocPlaceholder/following-sibling::bd:chapter | $tocPlaceholder/following-sibling::bd:include" mode="TOC" />
						</xsl:when>

						<xsl:otherwise>
							<xsl:apply-templates select="/bd:document/bd:part | /bd:document/bd:chapter | /bd:document/bd:include" mode="TOC" />
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>

				<xsl:otherwise>
					<xsl:choose>
						<xsl:when test="$tocPlaceholder">
							<xsl:apply-templates select="$tocPlaceholder/following-sibling::bd:chapter | $tocPlaceholder/following-sibling::bd:include" mode="TOC" />
						</xsl:when>

						<xsl:otherwise>
							<xsl:apply-templates select="/bd:document/bd:chapter | /bd:document/bd:include" mode="TOC" />
						</xsl:otherwise>
					</xsl:choose>

				</xsl:otherwise>
			</xsl:choose>
		</ul>

		<xsl:value-of select="$newline" />
	</xsl:template>

	<xsl:template match="bd:part" mode="TOC">
		<xsl:variable name="partCount">
			<xsl:number level="any" count="bd:part" />
		</xsl:variable>

		<li class="bdml-toc-part">
			<p class="bdml-toc-part-title">
				<a>
					<xsl:attribute name="href">
						<xsl:choose>
							<xsl:when test="@id">
								<!-- The element already has an Id.. reuse it.-->
								<xsl:value-of select="concat('#', @id)" />
							</xsl:when>

							<xsl:otherwise>
								<xsl:value-of select="concat('#Part_', $partCount, '_')" />
								<xsl:value-of select="translate(@title, '&lt;&gt;&amp;# ', '____-')" />
							</xsl:otherwise>
						</xsl:choose>
					</xsl:attribute>

					<xsl:value-of select="@title" />
				</a>
			</p>

			<xsl:value-of select="$newline" />

			<ul class="bdml-toc-part-ul">
				<xsl:value-of select="$newline" />

				<xsl:variable name="parentPart" select="." />

				<xsl:for-each select="bd:chapter | bd:include">
					<xsl:apply-templates select="." mode="TOC">
						<xsl:with-param name="parentPart" select="$parentPart" />
					</xsl:apply-templates>
				</xsl:for-each>
			</ul>

			<xsl:value-of select="$newline" />
		</li>

		<xsl:value-of select="$newline" />
	</xsl:template>

	<xsl:template match="bd:include" mode="TOC">
		<xsl:param name="parentPart" />

		<xsl:variable name="applies">
			<xsl:call-template name="target-and-variant-applies">
				<xsl:with-param name="target-list" select="@target" />
				<xsl:with-param name="variant-list" select="@variant" />
				<xsl:with-param name="theTarget" select="'html'" />
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="string($applies) = 'true'">
			<xsl:variable name="chapterCount">
				<xsl:number level="any" count="bd:chapter | bd:include[not(@variant)]" />
			</xsl:variable>

			<xsl:variable name="doc" select="document(concat($relativeRoot, @url), .)" />

			<xsl:if test="not($doc)">
				<xsl:call-template name="reportError">
					<xsl:with-param name="message" select="concat('the included document at ', $relativeRoot, @url, ' was not found.')" />
				</xsl:call-template>
			</xsl:if>

			<xsl:variable name="documentElement" select="$doc/bd:document" />

			<xsl:if test="not($documentElement)">
				<xsl:call-template name="reportError">
					<xsl:with-param name="message" select="concat('the document element of the included document at ', $relativeRoot, @url, ' was not found.')" />
				</xsl:call-template>
			</xsl:if>

			<xsl:apply-templates select="$documentElement/bd:chapter" mode="TOC">
				<xsl:with-param name="parentPart" select="$parentPart" />
				<xsl:with-param name="chapterCount" select="$chapterCount" />
			</xsl:apply-templates>
		</xsl:if>
	</xsl:template>

	<xsl:template match="bd:chapter" mode="TOC">
		<xsl:param name="parentPart" />

		<xsl:param name="chapterCount">
			<xsl:number level="any" count="bd:chapter | bd:include" />
		</xsl:param>

		<xsl:variable name="isAppendix" select="$chapterCount > $nonAppendixCount" />

		<xsl:variable name="realChapterCount">
			<xsl:choose>
				<xsl:when test="not($isAppendix)">
					<xsl:value-of select="$chapterCount - $frontMatterCount + $chapterStartOffset" />
				</xsl:when>

				<xsl:otherwise>
					<xsl:value-of select="substring($uppercase, $chapterCount - $nonAppendixCount + $chapterStartOffset, 1)" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:if test="$tocFinish >= 0 and $chapterCount >= $frontMatterCount">
			<li>
				<xsl:attribute name="class">
					<xsl:choose>
						<xsl:when test="contains(@class, 'bdml-after-toc-front-matter')">
							<xsl:value-of select="'bdml-toc-chapter bdml-after-toc-front-matter'" />

						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="'bdml-toc-chapter'" />
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>

				<xsl:if test="$tocStart &lt;= 0">
					<a>
						<xsl:attribute name="href">
							<xsl:choose>
								<xsl:when test="@id">
									<!-- The element already has an Id.. reuse it.-->
									<xsl:value-of select="concat('#', @id)" />
								</xsl:when>

								<xsl:otherwise>
									<xsl:value-of select="concat('#Chapter_', $realChapterCount, '_')" />

									<xsl:if test="$parentPart">
										<xsl:value-of select="translate($parentPart/@title, '&lt;&gt;&amp;# ', '____-')" />
										<xsl:value-of select="'__'" />
									</xsl:if>

									<xsl:value-of select="translate(@title, '&lt;&gt;&amp;# ', '____-')" />
								</xsl:otherwise>
							</xsl:choose>
						</xsl:attribute>

						<xsl:if test="$numberingStart &lt;= 0 and $numberingFinish >= 0 and ($isAppendix or $realChapterCount > 0)">
							<xsl:value-of select="$realChapterCount" />
							<xsl:text>. </xsl:text>
						</xsl:if>

						<xsl:value-of select="@title" />
					</a>
				</xsl:if>

				<!-- Do direct descendants -->
				<xsl:if test="bd:h1 | bd:section">
					<xsl:value-of select="$newline" />

					<ul class="bdml-toc-chapter-ul">
						<xsl:value-of select="$newline" />

						<xsl:variable name="parentChapter" select="." />

						<xsl:for-each select="bd:h1 | bd:section">
							<xsl:apply-templates select="." mode="TOC">
								<xsl:with-param name="parentPart" select="$parentPart" />
								<xsl:with-param name="parentChapter" select="$parentChapter" />
								<xsl:with-param name="chapterCount" select="$realChapterCount" />
							</xsl:apply-templates>
						</xsl:for-each>
					</ul>

					<xsl:value-of select="$newline" />
				</xsl:if>
			</li>

			<xsl:value-of select="$newline" />
		</xsl:if>
	</xsl:template>

	<xsl:template match="bd:chapter[contains(@class,'bdml-header') or contains(@class,'bdml-footer')]" mode="TOC">
	</xsl:template>

	<xsl:template match="bd:section" mode="TOC">
		<xsl:param name="parentPart" />
		<xsl:param name="parentChapter" />
		<xsl:param name="chapterCount" />

		<xsl:for-each select="bd:h1">
			<xsl:apply-templates select="." mode="TOC">
				<xsl:with-param name="parentPart" select="$parentPart" />
				<xsl:with-param name="parentChapter" select="$parentChapter" />
				<xsl:with-param name="chapterCount" select="$chapterCount" />
			</xsl:apply-templates>
		</xsl:for-each>
	</xsl:template>

	<xsl:template match="bd:h1" mode="TOC">
		<xsl:param name="parentPart" />
		<xsl:param name="parentChapter" />
		<xsl:param name="chapterCount" />

		<xsl:variable name="h1Index">
			<xsl:number level="any" from="bd:chapter" count="bd:h1" />
		</xsl:variable>

		<xsl:if test="$tocFinish >= 1 and (not(@toc) or @toc = 'true')">
			<li class="bdml-toc-h1">
				<xsl:if test="$tocStart &lt;= 1">
					<a>
						<xsl:attribute name="href">
							<xsl:choose>
								<xsl:when test="@id">
									<!-- The element already has an Id.. reuse it.-->
									<xsl:value-of select="concat('#', @id)" />
								</xsl:when>

								<xsl:otherwise>
									<xsl:value-of select="concat('#H1_', $chapterCount, '_', $h1Index, '_')" />

									<xsl:if test="$parentPart">
										<xsl:value-of select="translate($parentPart/@title, '&lt;&gt;&amp;# ', '____-')" />
										<xsl:value-of select="'__'" />
									</xsl:if>

									<xsl:value-of select="translate($parentChapter/@title, '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(., '&lt;&gt;&amp;# ', '____-')" />
								</xsl:otherwise>
							</xsl:choose>
						</xsl:attribute>

						<xsl:if test="$numberingStart &lt;= 1 and $numberingFinish >= 1 and (not(number($chapterCount) = $chapterCount) or $chapterCount > 0)">
							<xsl:value-of select="$chapterCount" />
							<xsl:text>.</xsl:text>
							<xsl:value-of select="$h1Index" />
							<xsl:text> </xsl:text>
						</xsl:if>

						<xsl:value-of select="." />
					</a>
				</xsl:if>

				<!-- Do direct descendants -->
				<xsl:variable name="directDescendants" select="following-sibling::bd:h2[generate-id(preceding-sibling::bd:h1[1]) = generate-id(current())]" />

				<xsl:if test="$directDescendants">
					<xsl:value-of select="$newline" />

					<ul class="bdml-toc-h1-ul">
						<xsl:value-of select="$newline" />

						<xsl:for-each select="$directDescendants">
							<xsl:apply-templates select="." mode="TOC">
								<xsl:with-param name="parentPart" select="$parentPart" />
								<xsl:with-param name="parentChapter" select="$parentChapter" />
								<xsl:with-param name="chapterCount" select="$chapterCount" />
							</xsl:apply-templates>
						</xsl:for-each>
					</ul>

					<xsl:value-of select="$newline" />
				</xsl:if>
			</li>

			<xsl:value-of select="$newline" />
		</xsl:if>
	</xsl:template>

	<xsl:template match="bd:h2" mode="TOC">
		<xsl:param name="parentPart" />
		<xsl:param name="parentChapter" />
		<xsl:param name="chapterCount" />

		<xsl:variable name="h1Index">
			<xsl:number level="any" from="bd:chapter" count="bd:h1" />
		</xsl:variable>

		<xsl:variable name="h2Index">
			<xsl:number level="any" from="bd:h1" count="bd:h2" />
		</xsl:variable>

		<xsl:if test="$tocFinish >= 2 and (not(@toc) or @toc = 'true')">
			<li class="bdml-toc-h2">
				<xsl:if test="$tocStart &lt;= 2">
					<a>
						<xsl:attribute name="href">
							<xsl:choose>
								<xsl:when test="@id">
									<!-- The element already has an Id.. reuse it.-->
									<xsl:value-of select="concat('#', @id)" />
								</xsl:when>

								<xsl:otherwise>
									<xsl:value-of select="concat('#H2_', $chapterCount, '_', $h1Index, '_', $h2Index, '_')" />

									<xsl:if test="$parentPart">
										<xsl:value-of select="translate($parentPart/@title, '&lt;&gt;&amp;# ', '____-')" />
										<xsl:value-of select="'__'" />
									</xsl:if>

									<xsl:value-of select="translate($parentChapter/@title, '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(preceding-sibling::bd:h1[1], '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(., '&lt;&gt;&amp;# ', '____-')" />
								</xsl:otherwise>
							</xsl:choose>
						</xsl:attribute>

						<xsl:if test="$numberingStart &lt;= 2 and $numberingFinish >= 2 and (not(number($chapterCount) = $chapterCount) or $chapterCount > 0)">
							<xsl:value-of select="$chapterCount" />
							<xsl:text>.</xsl:text>
							<xsl:number level="any" from="bd:chapter" count="bd:h1" />
							<xsl:text>.</xsl:text>
							<xsl:number level="any" from="bd:h1" count="bd:h2" />
							<xsl:text> </xsl:text>
						</xsl:if>

						<xsl:value-of select="." />
					</a>
				</xsl:if>

				<!-- Do direct descendants -->
				<xsl:variable name="directDescendants" select="following-sibling::bd:h3[generate-id(preceding-sibling::bd:h2[1]) = generate-id(current())]" />

				<xsl:if test="$directDescendants">
					<xsl:value-of select="$newline" />

					<ul class="bdml-toc-h2-ul">
						<xsl:value-of select="$newline" />

						<xsl:for-each select="$directDescendants">
							<xsl:apply-templates select="." mode="TOC">
								<xsl:with-param name="parentPart" select="$parentPart" />
								<xsl:with-param name="parentChapter" select="$parentChapter" />
								<xsl:with-param name="chapterCount" select="$chapterCount" />
							</xsl:apply-templates>
						</xsl:for-each>
					</ul>

					<xsl:value-of select="$newline" />
				</xsl:if>
			</li>

			<xsl:value-of select="$newline" />
		</xsl:if>
	</xsl:template>

	<xsl:template match="bd:h3" mode="TOC">
		<xsl:param name="parentPart" />
		<xsl:param name="parentChapter" />
		<xsl:param name="chapterCount" />

		<xsl:variable name="h1Index">
			<xsl:number level="any" from="bd:chapter" count="bd:h1" />
		</xsl:variable>

		<xsl:variable name="h2Index">
			<xsl:number level="any" from="bd:h1" count="bd:h2" />
		</xsl:variable>

		<xsl:variable name="h3Index">
			<xsl:number level="any" from="bd:h2" count="bd:h3" />
		</xsl:variable>

		<xsl:if test="$tocFinish >= 3 and (not(@toc) or @toc = 'true')">
			<li class="bdml-toc-h3">
				<xsl:if test="$tocStart &lt;= 3">
					<a>
						<xsl:attribute name="href">
							<xsl:choose>
								<xsl:when test="@id">
									<!-- The element already has an Id.. reuse it.-->
									<xsl:value-of select="concat('#', @id)" />
								</xsl:when>

								<xsl:otherwise>
									<xsl:value-of select="concat('#H3_', $chapterCount, '_', $h1Index, '_', $h2Index, '_', $h3Index, '_')" />

									<xsl:if test="$parentPart">
										<xsl:value-of select="translate($parentPart/@title, '&lt;&gt;&amp;# ', '____-')" />
										<xsl:value-of select="'__'" />
									</xsl:if>

									<xsl:value-of select="translate($parentChapter/@title, '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(preceding-sibling::bd:h1[1], '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(preceding-sibling::bd:h2[1], '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(., '&lt;&gt;&amp;# ', '____-')" />
								</xsl:otherwise>
							</xsl:choose>
						</xsl:attribute>

						<xsl:if test="$numberingStart &lt;= 3 and $numberingFinish >= 3 and (not(number($chapterCount) = $chapterCount) or $chapterCount > 0)">
							<xsl:value-of select="$chapterCount" />
							<xsl:text>.</xsl:text>
							<xsl:number level="any" from="bd:chapter" count="bd:h1" />
							<xsl:text>.</xsl:text>
							<xsl:number level="any" from="bd:h1" count="bd:h2" />
							<xsl:text>.</xsl:text>
							<xsl:number level="any" from="bd:h2" count="bd:h3" />
							<xsl:text> </xsl:text>
						</xsl:if>

						<xsl:value-of select="." />
					</a>
				</xsl:if>

				<!-- Do direct descendants -->
				<xsl:variable name="directDescendants" select="following-sibling::bd:h4[generate-id(preceding-sibling::bd:h3[1]) = generate-id(current())]" />

				<xsl:if test="$directDescendants">
					<xsl:value-of select="$newline" />

					<ul class="bdml-toc-h3-ul">
						<xsl:value-of select="$newline" />

						<xsl:for-each select="$directDescendants">
							<xsl:apply-templates select="." mode="TOC">
								<xsl:with-param name="parentPart" select="$parentPart" />
								<xsl:with-param name="parentChapter" select="$parentChapter" />
								<xsl:with-param name="chapterCount" select="$chapterCount" />
							</xsl:apply-templates>
						</xsl:for-each>
					</ul>

					<xsl:value-of select="$newline" />
				</xsl:if>
			</li>

			<xsl:value-of select="$newline" />
		</xsl:if>
	</xsl:template>

	<xsl:template match="bd:h4" mode="TOC">
		<xsl:param name="parentPart" />
		<xsl:param name="parentChapter" />
		<xsl:param name="chapterCount" />

		<xsl:variable name="h1Index">
			<xsl:number level="any" from="bd:chapter" count="bd:h1" />
		</xsl:variable>

		<xsl:variable name="h2Index">
			<xsl:number level="any" from="bd:h1" count="bd:h2" />
		</xsl:variable>

		<xsl:variable name="h3Index">
			<xsl:number level="any" from="bd:h2" count="bd:h3" />
		</xsl:variable>

		<xsl:variable name="h4Index">
			<xsl:number level="any" from="bd:h3" count="bd:h4" />
		</xsl:variable>

		<xsl:if test="$tocFinish >= 4 and (not(@toc) or @toc = 'true')">
			<li class="bdml-toc-h4">
				<xsl:if test="$tocStart &lt;= 4">
					<a>
						<xsl:attribute name="href">
							<xsl:choose>
								<xsl:when test="@id">
									<!-- The element already has an Id.. reuse it.-->
									<xsl:value-of select="concat('#', @id)" />
								</xsl:when>

								<xsl:otherwise>
									<xsl:value-of select="concat('#H4_', $chapterCount, '_', $h1Index, '_', $h2Index, '_', $h3Index, '_', $h4Index, '_')" />

									<xsl:if test="$parentPart">
										<xsl:value-of select="translate($parentPart/@title, '&lt;&gt;&amp;# ', '____-')" />
										<xsl:value-of select="'__'" />
									</xsl:if>

									<xsl:value-of select="translate($parentChapter/@title, '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(preceding-sibling::bd:h1[1], '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(preceding-sibling::bd:h2[1], '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(preceding-sibling::bd:h3[1], '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(., '&lt;&gt;&amp;# ', '____-')" />
								</xsl:otherwise>
							</xsl:choose>
						</xsl:attribute>

						<xsl:if test="$numberingStart &lt;= 4 and $numberingFinish >= 4 and (not(number($chapterCount) = $chapterCount) or $chapterCount > 0)">
							<xsl:value-of select="$chapterCount" />
							<xsl:text>.</xsl:text>
							<xsl:number level="any" from="bd:chapter" count="bd:h1" />
							<xsl:text>.</xsl:text>
							<xsl:number level="any" from="bd:h1" count="bd:h2" />
							<xsl:text>.</xsl:text>
							<xsl:number level="any" from="bd:h2" count="bd:h3" />
							<xsl:text>.</xsl:text>
							<xsl:number level="any" from="bd:h3" count="bd:h4" />
							<xsl:text> </xsl:text>
						</xsl:if>

						<xsl:value-of select="." />
					</a>
				</xsl:if>

				<!-- Do direct descendants -->
				<xsl:variable name="directDescendants" select="following-sibling::bd:h5[generate-id(preceding-sibling::bd:h4[1]) = generate-id(current())]" />

				<xsl:if test="$directDescendants">
					<xsl:value-of select="$newline" />

					<ul class="bdml-toc-h4-ul">
						<xsl:value-of select="$newline" />

						<xsl:for-each select="$directDescendants">
							<xsl:apply-templates select="." mode="TOC">
								<xsl:with-param name="parentPart" select="$parentPart" />
								<xsl:with-param name="parentChapter" select="$parentChapter" />
								<xsl:with-param name="chapterCount" select="$chapterCount" />
							</xsl:apply-templates>
						</xsl:for-each>
					</ul>

					<xsl:value-of select="$newline" />
				</xsl:if>
			</li>

			<xsl:value-of select="$newline" />
		</xsl:if>
	</xsl:template>

	<xsl:template match="bd:h5" mode="TOC">
		<xsl:param name="parentPart" />
		<xsl:param name="parentChapter" />
		<xsl:param name="chapterCount" />

		<xsl:variable name="h1Index">
			<xsl:number level="any" from="bd:chapter" count="bd:h1" />
		</xsl:variable>

		<xsl:variable name="h2Index">
			<xsl:number level="any" from="bd:h1" count="bd:h2" />
		</xsl:variable>

		<xsl:variable name="h3Index">
			<xsl:number level="any" from="bd:h2" count="bd:h3" />
		</xsl:variable>

		<xsl:variable name="h4Index">
			<xsl:number level="any" from="bd:h3" count="bd:h4" />
		</xsl:variable>

		<xsl:variable name="h5Index">
			<xsl:number level="any" from="bd:h4" count="bd:h5" />
		</xsl:variable>

		<xsl:if test="$tocFinish >= 5 and (not(@toc) or @toc = 'true')">
			<li class="bdml-toc-h5">
				<xsl:if test="$tocStart &lt;= 5">
					<a>
						<xsl:attribute name="href">
							<xsl:choose>
								<xsl:when test="@id">
									<!-- The element already has an Id.. reuse it.-->
									<xsl:value-of select="concat('#', @id)" />
								</xsl:when>

								<xsl:otherwise>
									<xsl:value-of select="concat('#H5_', $chapterCount, '_', $h1Index, '_', $h2Index, '_', $h3Index, '_', $h4Index, '_', $h5Index, '_')" />

									<xsl:if test="$parentPart">
										<xsl:value-of select="translate($parentPart/@title, '&lt;&gt;&amp;# ', '____-')" />
										<xsl:value-of select="'__'" />
									</xsl:if>

									<xsl:value-of select="translate($parentChapter/@title, '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(preceding-sibling::bd:h1[1], '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(preceding-sibling::bd:h2[1], '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(preceding-sibling::bd:h3[1], '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(preceding-sibling::bd:h4[1], '&lt;&gt;&amp;# ', '____-')" />
									<xsl:value-of select="'__'" />

									<xsl:value-of select="translate(., '&lt;&gt;&amp;# ', '____-')" />
								</xsl:otherwise>
							</xsl:choose>
						</xsl:attribute>

						<xsl:if test="$numberingStart &lt;= 5 and $numberingFinish >= 5 and (not(number($chapterCount) = $chapterCount) or $chapterCount > 0)">
							<xsl:value-of select="$chapterCount" />
							<xsl:text>.</xsl:text>
							<xsl:number level="any" from="bd:chapter" count="bd:h1" />
							<xsl:text>.</xsl:text>
							<xsl:number level="any" from="bd:h1" count="bd:h2" />
							<xsl:text>.</xsl:text>
							<xsl:number level="any" from="bd:h2" count="bd:h3" />
							<xsl:text>.</xsl:text>
							<xsl:number level="any" from="bd:h3" count="bd:h4" />
							<xsl:text>.</xsl:text>
							<xsl:number level="any" from="bd:h4" count="bd:h5" />
							<xsl:text> </xsl:text>
						</xsl:if>

						<xsl:value-of select="." />
					</a>
				</xsl:if>
			</li>

			<xsl:value-of select="$newline" />
		</xsl:if>
	</xsl:template>

	<!-- ************************ MAIN MODE *********************** -->

	<xsl:template match="bd:part">
		<xsl:variable name="partCount">
			<xsl:number level="any" count="bd:part" />
		</xsl:variable>

		<div class="bdml-part">
			<xsl:call-template name="copyClassAttribute" />

			<xsl:attribute name="id">
				<xsl:choose>
					<xsl:when test="@id">
						<!-- The element already has an Id.. reuse it.-->
						<xsl:value-of select="@id" />
					</xsl:when>

					<xsl:otherwise>
						<xsl:value-of select="concat('Part_', $partCount, '_')" />
						<xsl:value-of select="translate(@title, '&lt;&gt;&amp;# ', '____-')" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>

			<div class="bdml-part-title">
				<xsl:value-of select="@title" />
			</div>

			<xsl:variable name="parentPart" select="." />

			<xsl:for-each select="bd:chapter | bd:include">
				<xsl:apply-templates select=".">
					<xsl:with-param name="parentPart" select="$parentPart" />
				</xsl:apply-templates>
			</xsl:for-each>
		</div>
	</xsl:template>

	<xsl:template match="bd:include">
		<xsl:param name="parentPart" />

		<xsl:variable name="applies">
			<xsl:call-template name="target-and-variant-applies">
				<xsl:with-param name="target-list" select="@target" />
				<xsl:with-param name="variant-list" select="@variant" />
				<xsl:with-param name="theTarget" select="'html'" />
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="string($applies) = 'true'">
			<xsl:variable name="chapterCount">
				<xsl:number level="any" count="bd:chapter | bd:include[not(@variant)]"  />
			</xsl:variable>

			<xsl:variable name="doc" select="document(concat($relativeRoot, @url), .)" />

			<xsl:if test="not($doc)">
				<xsl:call-template name="reportError">
					<xsl:with-param name="message" select="concat('the included document at ', $relativeRoot, @url, ' was not found.')" />
				</xsl:call-template>
			</xsl:if>

			<xsl:variable name="documentElement" select="$doc/bd:document" />

			<xsl:if test="not($documentElement)">
				<xsl:call-template name="reportError">
					<xsl:with-param name="message" select="concat('the document element of the included document at ', $relativeRoot, @url, ' was not found.')" />
				</xsl:call-template>
			</xsl:if>

			<xsl:if test="not($documentElement/bd:chapter)">
				<xsl:call-template name="reportError">
					<xsl:with-param name="message" select="concat('there is no chapter element in the included document at ', $relativeRoot, @url, '.')" />
				</xsl:call-template>
			</xsl:if>

			<xsl:apply-templates select="$documentElement/bd:chapter">
				<xsl:with-param name="parentPart" select="$parentPart" />
				<xsl:with-param name="chapterCount" select="$chapterCount" />
			</xsl:apply-templates>
		</xsl:if>
	</xsl:template>

	<xsl:template match="bd:chapter">
		<xsl:param name="parentPart" />

		<xsl:param name="chapterCount">
			<xsl:number level="any" count="bd:chapter | bd:include" />
		</xsl:param>

		<xsl:variable name="isAppendix" select="$chapterCount > $nonAppendixCount" />

		<xsl:variable name="realChapterCount">
			<xsl:choose>
				<xsl:when test="not($isAppendix)">
					<xsl:value-of select="$chapterCount - $frontMatterCount + $chapterStartOffset" />
				</xsl:when>

				<xsl:otherwise>
					<xsl:value-of select="substring($uppercase, $chapterCount - $nonAppendixCount + $chapterStartOffset, 1)" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<div class="bdml-chapter">
			<xsl:call-template name="copyClassAttribute" />

			<xsl:attribute name="id">
				<xsl:choose>
					<xsl:when test="@id">
						<!-- The element already has an Id.. reuse it.-->
						<xsl:value-of select="@id" />
					</xsl:when>

					<xsl:otherwise>
						<xsl:value-of select="concat('Chapter_', $realChapterCount, '_')" />

						<xsl:if test="$parentPart">
							<xsl:value-of select="translate($parentPart/@title, '&lt;&gt;&amp;# ', '____-')" />
							<xsl:value-of select="'__'" />
						</xsl:if>

						<xsl:value-of select="translate(@title, '&lt;&gt;&amp;# ', '____-')" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>

			<xsl:attribute name="numbering">
				<xsl:value-of select="$realChapterCount" />
				<xsl:text>.</xsl:text>
			</xsl:attribute>

			<xsl:if test="*[1][contains(@class,'bdml-link')]">
				<span class="{*[1][contains(@class,'bdml-link')]/@class}" id="{*[1][contains(@class,'bdml-link')]/@id}">
				</span>
			</xsl:if>

			<div class="bdml-chapter-title">
				<xsl:if test="$numberingStart &lt;= 0 and $numberingFinish >= 0 and ($isAppendix or $realChapterCount > 0)">
					<xsl:value-of select="$realChapterCount" />
					<xsl:text>.</xsl:text>
					<xsl:text> </xsl:text>
				</xsl:if>

				<xsl:value-of select="@title" />
			</div>

			<xsl:variable name="parentChapter" select="." />

			<xsl:for-each select="* | node()">
				<xsl:apply-templates select=".">
					<xsl:with-param name="parentPart" select="$parentPart" />
					<xsl:with-param name="parentChapter" select="$parentChapter" />
					<xsl:with-param name="chapterCount" select="$realChapterCount" />
				</xsl:apply-templates>
			</xsl:for-each>
		</div>
	</xsl:template>

	<xsl:template match="bd:chapter[contains(@class,'bdml-header') or contains(@class,'bdml-footer')]">
		<div class="bdml-chapter">

			<xsl:call-template name="copyClassAttribute" />
			<xsl:for-each select="* | node()">
				<xsl:apply-templates select=".">
				</xsl:apply-templates>
			</xsl:for-each>

		</div>
	</xsl:template>

	<xsl:template match="bd:section">
		<xsl:param name="parentPart" />
		<xsl:param name="parentChapter" />
		<xsl:param name="chapterCount" />

		<div>
			<xsl:call-template name="copyClassAndIdAttributes" />

			<xsl:apply-templates>
				<xsl:with-param name="parentPart" select="$parentPart" />
				<xsl:with-param name="parentChapter" select="$parentChapter" />
				<xsl:with-param name="chapterCount" select="$chapterCount" />
			</xsl:apply-templates>
		</div>
	</xsl:template>

	<xsl:template match="bd:h1">
		<xsl:param name="parentPart" />
		<xsl:param name="parentChapter" />
		<xsl:param name="chapterCount" />

		<xsl:variable name="h1Index">
			<xsl:number level="any" from="bd:chapter" count="bd:h1" />
		</xsl:variable>

		<h1>
			<xsl:attribute name="id">
				<xsl:choose>
					<xsl:when test="@id">
						<!-- The element already has an Id.. reuse it.-->
						<xsl:value-of select="@id" />
					</xsl:when>

					<xsl:otherwise>
						<xsl:value-of select="concat('H1_', $chapterCount, '_', $h1Index, '_')" />

						<xsl:if test="$parentPart">
							<xsl:value-of select="translate($parentPart/@title, '&lt;&gt;&amp;# ', '____-')" />
							<xsl:value-of select="'__'" />
						</xsl:if>

						<xsl:value-of select="translate($parentChapter/@title, '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(., '&lt;&gt;&amp;# ', '____-')" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>

			<xsl:attribute name="numbering">
				<xsl:value-of select="$chapterCount" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h1Index" />
			</xsl:attribute>

			<xsl:call-template name="copyClassAttribute" />

			<xsl:if test="$numberingStart &lt;= 1 and $numberingFinish >= 1 and (not(number($chapterCount) = $chapterCount) or $chapterCount > 0)">
				<xsl:value-of select="$chapterCount" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h1Index" />
				<xsl:text> </xsl:text>
			</xsl:if>

			<xsl:copy-of select="node()" />
		</h1>
	</xsl:template>

	<xsl:template match="bd:h2">
		<xsl:param name="parentPart" />
		<xsl:param name="parentChapter" />
		<xsl:param name="chapterCount" />

		<xsl:variable name="h1Index">
			<xsl:number level="any" from="bd:chapter" count="bd:h1" />
		</xsl:variable>

		<xsl:variable name="h2Index">
			<xsl:number level="any" from="bd:h1" count="bd:h2" />
		</xsl:variable>

		<h2>
			<xsl:attribute name="id">
				<xsl:choose>
					<xsl:when test="@id">
						<!-- The element already has an Id.. reuse it.-->
						<xsl:value-of select="@id" />
					</xsl:when>

					<xsl:otherwise>
						<xsl:value-of select="concat('H2_', $chapterCount, '_', $h1Index, '_', $h2Index, '_')" />

						<xsl:if test="$parentPart">
							<xsl:value-of select="translate($parentPart/@title, '&lt;&gt;&amp;# ', '____-')" />
							<xsl:value-of select="'__'" />
						</xsl:if>

						<xsl:value-of select="translate($parentChapter/@title, '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(preceding-sibling::bd:h1[1], '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(., '&lt;&gt;&amp;# ', '____-')" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>

			<xsl:attribute name="numbering">
				<xsl:value-of select="$chapterCount" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h1Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h2Index" />
			</xsl:attribute>

			<xsl:call-template name="copyClassAttribute" />

			<xsl:if test="$numberingStart &lt;= 2 and $numberingFinish >= 2 and (not(number($chapterCount) = $chapterCount) or $chapterCount > 0)">
				<xsl:value-of select="$chapterCount" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h1Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h2Index" />
				<xsl:text> </xsl:text>
			</xsl:if>

			<xsl:copy-of select="node()" />
		</h2>
	</xsl:template>

	<xsl:template match="bd:h3">
		<xsl:param name="parentPart" />
		<xsl:param name="parentChapter" />
		<xsl:param name="chapterCount" />

		<xsl:variable name="h1Index">
			<xsl:number level="any" from="bd:chapter" count="bd:h1" />
		</xsl:variable>

		<xsl:variable name="h2Index">
			<xsl:number level="any" from="bd:h1" count="bd:h2" />
		</xsl:variable>

		<xsl:variable name="h3Index">
			<xsl:number level="any" from="bd:h2" count="bd:h3" />
		</xsl:variable>

		<h3>
			<xsl:attribute name="id">
				<xsl:choose>
					<xsl:when test="@id">
						<!-- The element already has an Id.. reuse it.-->
						<xsl:value-of select="@id" />
					</xsl:when>

					<xsl:otherwise>
						<xsl:value-of select="concat('H3_', $chapterCount, '_', $h1Index, '_', $h2Index, '_', $h3Index, '_')" />

						<xsl:if test="$parentPart">
							<xsl:value-of select="translate($parentPart/@title, '&lt;&gt;&amp;# ', '____-')" />
							<xsl:value-of select="'__'" />
						</xsl:if>

						<xsl:value-of select="translate($parentChapter/@title, '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(preceding-sibling::bd:h1[1], '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(preceding-sibling::bd:h2[1], '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(., '&lt;&gt;&amp;# ', '____-')" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>

			<xsl:attribute name="numbering">
				<xsl:value-of select="$chapterCount" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h1Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h2Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h3Index" />
			</xsl:attribute>

			<xsl:call-template name="copyClassAttribute" />

			<xsl:if test="$numberingStart &lt;= 3 and $numberingFinish >= 3 and (not(number($chapterCount) = $chapterCount) or $chapterCount > 0)">
				<xsl:value-of select="$chapterCount" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h1Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h2Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h3Index" />
				<xsl:text> </xsl:text>
			</xsl:if>

			<xsl:copy-of select="node()" />
		</h3>
	</xsl:template>

	<xsl:template match="bd:h4">
		<xsl:param name="parentPart" />
		<xsl:param name="parentChapter" />
		<xsl:param name="chapterCount" />

		<xsl:variable name="h1Index">
			<xsl:number level="any" from="bd:chapter" count="bd:h1" />
		</xsl:variable>

		<xsl:variable name="h2Index">
			<xsl:number level="any" from="bd:h1" count="bd:h2" />
		</xsl:variable>

		<xsl:variable name="h3Index">
			<xsl:number level="any" from="bd:h2" count="bd:h3" />
		</xsl:variable>

		<xsl:variable name="h4Index">
			<xsl:number level="any" from="bd:h3" count="bd:h4" />
		</xsl:variable>

		<h4>
			<xsl:attribute name="id">
				<xsl:choose>
					<xsl:when test="@id">
						<!-- The element already has an Id.. reuse it.-->
						<xsl:value-of select="@id" />
					</xsl:when>

					<xsl:otherwise>
						<xsl:value-of select="concat('H4_', $chapterCount, '_', $h1Index, '_', $h2Index, '_', $h3Index, '_', $h4Index, '_')" />

						<xsl:if test="$parentPart">
							<xsl:value-of select="translate($parentPart/@title, '&lt;&gt;&amp;# ', '____-')" />
							<xsl:value-of select="'__'" />
						</xsl:if>

						<xsl:value-of select="translate($parentChapter/@title, '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(preceding-sibling::bd:h1[1], '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(preceding-sibling::bd:h2[1], '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(preceding-sibling::bd:h3[1], '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(., '&lt;&gt;&amp;# ', '____-')" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>

			<xsl:attribute name="numbering">
				<xsl:value-of select="$chapterCount" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h1Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h2Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h3Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h4Index" />
			</xsl:attribute>

			<xsl:call-template name="copyClassAttribute" />

			<xsl:if test="$numberingStart &lt;= 4 and $numberingFinish >= 4 and (not(number($chapterCount) = $chapterCount) or $chapterCount > 0)">
				<xsl:value-of select="$chapterCount" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h1Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h2Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h3Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h4Index" />
				<xsl:text> </xsl:text>
			</xsl:if>

			<xsl:copy-of select="node()" />
		</h4>
	</xsl:template>

	<xsl:template match="bd:h5">
		<xsl:param name="parentPart" />
		<xsl:param name="parentChapter" />
		<xsl:param name="chapterCount" />

		<xsl:variable name="h1Index">
			<xsl:number level="any" from="bd:chapter" count="bd:h1" />
		</xsl:variable>

		<xsl:variable name="h2Index">
			<xsl:number level="any" from="bd:h1" count="bd:h2" />
		</xsl:variable>

		<xsl:variable name="h3Index">
			<xsl:number level="any" from="bd:h2" count="bd:h3" />
		</xsl:variable>

		<xsl:variable name="h4Index">
			<xsl:number level="any" from="bd:h3" count="bd:h4" />
		</xsl:variable>

		<xsl:variable name="h5Index">
			<xsl:number level="any" from="bd:h4" count="bd:h5" />
		</xsl:variable>

		<h5>
			<xsl:attribute name="id">
				<xsl:choose>
					<xsl:when test="@id">
						<!-- The element already has an Id.. reuse it.-->
						<xsl:value-of select="@id" />
					</xsl:when>

					<xsl:otherwise>
						<xsl:value-of select="concat('H5_', $chapterCount, '_', $h1Index, '_', $h2Index, '_', $h3Index, '_', $h4Index, '_', $h5Index, '_')" />

						<xsl:if test="$parentPart">
							<xsl:value-of select="translate($parentPart/@title, '&lt;&gt;&amp;# ', '____-')" />
							<xsl:value-of select="'__'" />
						</xsl:if>

						<xsl:value-of select="translate($parentChapter/@title, '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(preceding-sibling::bd:h1[1], '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(preceding-sibling::bd:h2[1], '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(preceding-sibling::bd:h3[1], '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(preceding-sibling::bd:h4[1], '&lt;&gt;&amp;# ', '____-')" />
						<xsl:value-of select="'__'" />

						<xsl:value-of select="translate(., '&lt;&gt;&amp;# ', '____-')" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>

			<xsl:attribute name="numbering">
				<xsl:value-of select="$chapterCount" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h1Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h2Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h3Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h4Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h5Index" />
			</xsl:attribute>

			<xsl:call-template name="copyClassAttribute" />

			<xsl:if test="$numberingStart &lt;= 5 and $numberingFinish >= 5 and (not(number($chapterCount) = $chapterCount) or $chapterCount > 0)">
				<xsl:value-of select="$chapterCount" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h1Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h2Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h3Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h4Index" />
				<xsl:text>.</xsl:text>
				<xsl:value-of select="$h5Index" />
				<xsl:text> </xsl:text>
			</xsl:if>

			<xsl:copy-of select="node()" />
		</h5>
	</xsl:template>

	<xsl:template match="bd:metadata" >
	</xsl:template>

	<xsl:template match="bd:para">
		<p>
			<xsl:call-template name="copyClassAndIdAttributes" />
			<xsl:apply-templates/>
		</p>
	</xsl:template>

	<xsl:template match="bd:para[contains(@class,'bdml-link')]">
	</xsl:template>

	<xsl:template match="bd:page-number">
		<span class="bdml-page-number">
			<xsl:call-template name="copyClassAndIdAttributes" />
			<xsl:apply-templates/>
			<xsl:if test="@target">
				<xsl:attribute name="href">
					<xsl:value-of select="@target"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:choose>
				<xsl:when test="@type">
					<xsl:attribute name="type">
						<xsl:value-of select="@type" />
					</xsl:attribute>
				</xsl:when>

				<xsl:otherwise>
					<xsl:attribute name="type">decimal</xsl:attribute>
				</xsl:otherwise>
			</xsl:choose>
		</span>
	</xsl:template>

	<xsl:template match="bd:page-count">
		<span class="bdml-page-count">
			<xsl:call-template name="copyClassAndIdAttributes" />
			<xsl:apply-templates/>
			<xsl:choose>
				<xsl:when test="@type">
					<xsl:attribute name="type">
						<xsl:value-of select="@type" />
					</xsl:attribute>
				</xsl:when>

				<xsl:otherwise>
					<xsl:attribute name="type">decimal</xsl:attribute>
				</xsl:otherwise>
			</xsl:choose>
		</span>
	</xsl:template>

	<xsl:template match="bd:newline">
		<br />
	</xsl:template>

	<xsl:template name="loremRender">
		<xsl:param name="remainder" />

		<xsl:choose>
			<xsl:when test="$remainder = 0">
			</xsl:when>

			<xsl:when test="$remainder = 445">
				<xsl:value-of select="'Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.'" />
			</xsl:when>

			<xsl:when test="$remainder &lt; 445">
				<xsl:value-of select="substring('Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.', 1, $remainder)" />
			</xsl:when>

			<xsl:otherwise> <!-- gt 445 -->
				<xsl:value-of select="'Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. '" />

				<xsl:call-template name="loremRender">
					<xsl:with-param name="remainder" select="$remainder - 446" />
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="bd:lorem">
		<p class="bdml-lorem">
			<xsl:choose>
				<xsl:when test="@length">
					<xsl:choose>
						<xsl:when test="@repeat">
							<xsl:choose>
								<!-- Limit repeat to something reasonable in order to prevent stack overflow. -->
								<xsl:when test="number(@repeat) * number(@length) > 44500">
									<xsl:call-template name="loremRender">
										<xsl:with-param name="remainder" select="44500" />
									</xsl:call-template>
									<xsl:value-of select="' -- lorem output limited to 44500 characters --'" />
								</xsl:when>

								<xsl:otherwise>
									<xsl:call-template name="loremRender">
										<xsl:with-param name="remainder" select="number(@repeat) * number(@length)" />
									</xsl:call-template>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>

						<xsl:otherwise>
							<xsl:choose>
								<!-- Limit repeat to something reasonable in order to prevent stack overflow. -->
								<xsl:when test="number(@length) > 44500">
									<xsl:call-template name="loremRender">
										<xsl:with-param name="remainder" select="44500" />
									</xsl:call-template>
									<xsl:value-of select="' -- lorem output limited to 44500 characters --'" />
								</xsl:when>

								<xsl:otherwise>
									<xsl:call-template name="loremRender">
										<xsl:with-param name="remainder" select="number(@length)" />
									</xsl:call-template>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>

				<xsl:when test="@repeat">
					<xsl:choose>
						<!-- Limit repeat to something reasonable in order to prevent stack overflow. -->
						<xsl:when test="number(@repeat) > 100">
							<xsl:call-template name="loremRender">
								<xsl:with-param name="remainder" select="44500" />
							</xsl:call-template>
							<xsl:value-of select="' -- lorem output limited to 100 repeats --'" />
						</xsl:when>

						<xsl:otherwise>
							<xsl:call-template name="loremRender">
								<xsl:with-param name="remainder" select="445 * number(@repeat)" />
							</xsl:call-template>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>

				<xsl:otherwise>
					<xsl:call-template name="loremRender">
						<xsl:with-param name="remainder" select="445" />
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</p>
	</xsl:template>

	<xsl:template match="bd:strong">
		<span class="bdml-strong">
			<xsl:call-template name="copyClassAndIdAttributes" />
			<xsl:apply-templates/>
		</span>
	</xsl:template>

	<xsl:template match="bd:emph">
		<span class="bdml-emph">
			<xsl:call-template name="copyClassAndIdAttributes" />
			<xsl:apply-templates/>
		</span>
	</xsl:template>

	<xsl:template match="bd:sup">
		<sup>
			<xsl:call-template name="copyClassAndIdAttributes" />
			<xsl:apply-templates/>
		</sup>
	</xsl:template>

	<xsl:template match="bd:sub">
		<sub>
			<xsl:call-template name="copyClassAndIdAttributes" />
			<xsl:apply-templates/>
		</sub>
	</xsl:template>

	<xsl:template match="bd:span">
		<span>
			<xsl:call-template name="copyClassAndIdAttributes" />
			<xsl:apply-templates/>
		</span>
	</xsl:template>

	<xsl:template match="bd:ref">
		<xsl:variable name="extension">
			<xsl:choose>
				<xsl:when test="$no-link-conversion = 'true'">
					<xsl:value-of select="$bdmlFileExtension" />
				</xsl:when>

				<xsl:otherwise>
					<xsl:value-of select="'.html'" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="base">
			<xsl:choose>
				<xsl:when test="contains(@url, '#')">
					<!-- Relative file path? Need docroot prefix. -->
					<xsl:if test="not(contains(@url, '://') or starts-with(@url, '/'))">
						<xsl:value-of select="$relativeRoot" />
					</xsl:if>

					<xsl:value-of select="substring-before(@url, '#')" />
				</xsl:when>

				<xsl:otherwise>
					<!-- Relative file path? Need docroot prefix. -->
					<xsl:if test="not(contains(@url, '://') or starts-with(@url, '/'))">
						<xsl:value-of select="$relativeRoot" />
					</xsl:if>

					<xsl:value-of select="@url" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="frag" select="substring-after(@url, '#')" />

		<xsl:variable name="link">
			<xsl:choose>
				<xsl:when test="contains(@type , 'raw')">
					<xsl:value-of select="@url" />
				</xsl:when>

				<xsl:when test="$frag != '' and $base != ''">
					<xsl:value-of select="concat($base, $extension, '#', $frag)" />
				</xsl:when>

				<xsl:when test="$frag != '' and $base = ''">
					<xsl:value-of select="concat('#', $frag)" />
				</xsl:when>

				<xsl:otherwise>
					<!-- $frag == '' and $base != '' -->
					<xsl:value-of select="concat($base, $extension)" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="new-window" select="@new" />

		<a>
			<xsl:if test="@title">
				<xsl:attribute name="title">
					<xsl:value-of select="@title" />
				</xsl:attribute>
			</xsl:if>

			<xsl:if test="@numbering">
				<xsl:attribute name="numbering">
					<xsl:value-of select="@numbering" />
				</xsl:attribute>
			</xsl:if>

			<xsl:if test="@prefix">
				<xsl:attribute name="prefix">
					<xsl:value-of select="@prefix" />
				</xsl:attribute>
			</xsl:if>

			<xsl:if test="@suffix">
				<xsl:attribute name="suffix">
					<xsl:value-of select="@suffix" />
				</xsl:attribute>
			</xsl:if>

			<xsl:attribute name="href">
				<xsl:value-of select="$link" />
			</xsl:attribute>

			<xsl:if test="$new-window = 'true'">
				<xsl:attribute name="target">
					<xsl:value-of select="'_blank'" />
				</xsl:attribute>
			</xsl:if>

			<!-- Use the link if there is no content. -->
			<xsl:choose>
				<xsl:when test="not(node()[not(self::comment())]) and not(@disable-repl)">
					<xsl:value-of select="$link" />
				</xsl:when>

				<xsl:otherwise>
					<xsl:apply-templates/>
				</xsl:otherwise>
			</xsl:choose>
		</a>
	</xsl:template>

	<xsl:template match="bd:bullets">
		<ul>
			<xsl:call-template name="copyClassAndIdAttributes" />
			<xsl:apply-templates select="bd:entry" />
		</ul>

		<xsl:value-of select="$newline" />
	</xsl:template>

	<xsl:template match="bd:list">
		<ol>
			<xsl:call-template name="copyClassAndIdAttributes" />

			<xsl:attribute name="start">
				<xsl:choose>
					<xsl:when test="@start">
						<xsl:value-of select="@start" />
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="'1'" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>

			<xsl:apply-templates select="bd:entry" />
		</ol>
	</xsl:template>

	<xsl:template match="bd:entry">
		<li>
			<xsl:call-template name="copyClassAndIdAttributes" />
			<xsl:apply-templates/>
		</li>

		<xsl:value-of select="$newline" />
	</xsl:template>

	<xsl:template match="bd:table">
		<xsl:variable name="colCount">
			<xsl:call-template name="colCounter" />
		</xsl:variable>

		<table>
			<xsl:call-template name="copyClassAndIdAttributes">
				<xsl:with-param name="extraClasses" select="'bdml-table'" />
			</xsl:call-template>

			<xsl:apply-templates select="bd:title" mode="table" />

			<colgroup>
				<xsl:call-template name="createColspecs">
					<xsl:with-param name="m" select="1" />
					<xsl:with-param name="colCount" select="$colCount" />
				</xsl:call-template>
			</colgroup>

			<xsl:apply-templates select="*[not(self::bd:title)]" mode="table" />
		</table>
	</xsl:template>

	<xsl:template name="colCounter">
		<xsl:variable name="tableRows" select="bd:head/bd:row | bd:body/bd:row" />

		<xsl:choose>
			<xsl:when test="$tableRows">
				<xsl:apply-templates select="$tableRows[1]" mode="tableColumnCount">
					<xsl:with-param name="tableRows" select="$tableRows" />
					<xsl:with-param name="rowIndex" select="1" />
					<xsl:with-param name="maxCount" select="0" />
				</xsl:apply-templates>
			</xsl:when>

			<xsl:otherwise>
				<xsl:value-of select="0" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="bd:row" mode="tableColumnCount">
		<xsl:param name="tableRows" />
		<xsl:param name="rowIndex" />
		<xsl:param name="maxCount" />

		<xsl:variable name="count" select="count(bd:cell)" />
		<xsl:variable name="newMaxCount" select="($maxCount >= $count) * $maxCount + ($maxCount &lt; $count) * $count" />

		<xsl:choose>
			<xsl:when test="$rowIndex = count($tableRows)">
				<xsl:value-of select="$newMaxCount" />
			</xsl:when>

			<xsl:otherwise>
				<xsl:apply-templates select="$tableRows[$rowIndex + 1]" mode="tableColumnCount">
					<xsl:with-param name="tableRows" select="$tableRows" />
					<xsl:with-param name="rowIndex" select="$rowIndex + 1" />
					<xsl:with-param name="maxCount" select="$newMaxCount" />
				</xsl:apply-templates>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="createColspecs">
		<xsl:param name="m" />
		<xsl:param name="colCount" />

		<xsl:if test="$m &lt;= $colCount">
			<col class="bdml-col{$m}" />

			<xsl:call-template name="createColspecs">
				<xsl:with-param name="m" select="$m + 1" />
				<xsl:with-param name="colCount" select="$colCount" />
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="bd:title" mode="table">
		<caption>
			<xsl:apply-templates/>
		</caption>
	</xsl:template>

	<xsl:template match="bd:head" mode="table">
		<thead class="bdml-menu-header">
			<tr>
				<xsl:apply-templates mode="tableHead" />
			</tr>
		</thead>
	</xsl:template>

	<xsl:template match="bd:body" mode="table">
		<tbody>
			<xsl:apply-templates mode="table" />
		</tbody>
	</xsl:template>

	<xsl:template match="bd:cell" mode="tableHead">
		<th>
			<xsl:call-template name="copyClassAndIdAttributes" />
			<xsl:if test="@span">
				<xsl:attribute name="colspan">
					<xsl:value-of select="@span" />
				</xsl:attribute>
			</xsl:if>
			<xsl:apply-templates/>
		</th>
	</xsl:template>

	<xsl:template match="bd:row" mode="table">
		<tr>
			<xsl:call-template name="copyClassAndIdAttributes" />
			<xsl:apply-templates mode="table" />
		</tr>
	</xsl:template>

	<xsl:template match="bd:cell" mode="table">
		<td>
			<xsl:call-template name="copyClassAndIdAttributes" />

			<xsl:if test="@span">
				<xsl:attribute name="colspan">
					<xsl:value-of select="@span" />
				</xsl:attribute>
			</xsl:if>

			<xsl:apply-templates />
		</td>
	</xsl:template>

	<!-- Helper template to create width and height attributes. Converts image dimensions to screen pixels. -->
	<xsl:template name="resolveMeasurement">
		<xsl:param name="widthOrHeight" />
		<xsl:param name="value" />
		<xsl:param name="dpcOrDpi" />
		<xsl:param name="dpx" />

		<xsl:attribute name="{$widthOrHeight}">
			<xsl:choose>
				<!-- Simulate ends-with($val, '%') -->
				<xsl:when test="substring($value, (string-length($value) - string-length('%')) + 1) = '%'">
					<xsl:value-of select="concat(string(number(substring-before($value, '%'))), '%')" />
				</xsl:when>

				<!-- Simulate ends-with($val, 'mm') -->
				<xsl:when test="substring($value, (string-length($value) - string-length('mm')) + 1) = 'mm'">
					<xsl:value-of select="round(number(substring-before($value, 'mm')) * 96 div 25.4)" />
				</xsl:when>

				<!-- Simulate ends-with($val, 'cm') -->
				<xsl:when test="substring($value, (string-length($value) - string-length('cm')) + 1) = 'cm'">
					<xsl:value-of select="round(number(substring-before($value, 'cm')) * 96 div 2.54)" />
				</xsl:when>

				<!-- Simulate ends-with($val, 'pt') -->
				<xsl:when test="substring($value, (string-length($value) - string-length('pt')) + 1) = 'pt'">
					<xsl:value-of select="round(number(substring-before($value, 'pt')) * 96 div 72)" />
				</xsl:when>

				<!-- Simulate ends-with($val, 'px') for dpc -->
				<xsl:when test="$dpcOrDpi = 'dpc' and substring($value, (string-length($value) - string-length('px')) + 1) = 'px'">
					<xsl:value-of select="round(substring-before($value, 'px') div $dpx * 96 * 2.54)" />
				</xsl:when>

				<!-- Simulate ends-with($val, 'px') for dpi -->
				<xsl:when test="$dpcOrDpi = 'dpi' and substring($value, (string-length($value) - string-length('px')) + 1) = 'px'">
					<xsl:value-of select="round(substring-before($value, 'px') div $dpx * 96)" />
				</xsl:when>

				<!-- also pixels, dpc -->
				<xsl:when test="$dpcOrDpi = 'dpc' and substring($value, (string-length($value) - string-length('px')) + 1) = 'px'">
					<xsl:value-of select="round($value div $dpx * 96 * 2.54)" />
				</xsl:when>

				<xsl:otherwise> <!-- also pixels, dpi -->
					<xsl:value-of select="round($value div $dpx * 96)" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:attribute>
	</xsl:template>

	<xsl:template match="bd:cover">
		<xsl:call-template name="drawGraphic">
			<xsl:with-param name="element-dpc" select="@dpc" />
			<xsl:with-param name="element-dpi" select="@dpi" />
			<xsl:with-param name="element-position" select="@position" />
			<xsl:with-param name="element-url" select="@url" />
			<xsl:with-param name="element-width" select="@width" />
			<xsl:with-param name="element-height" select="@height" />
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="bd:graphic">
		<xsl:choose>
			<xsl:when test="bd:target">
				<xsl:variable name="qualifying-targets">
					<xsl:call-template name="applicable-targets-and-variants">
						<xsl:with-param name="elements" select="bd:target" />
						<xsl:with-param name="theTarget" select="$target" />
					</xsl:call-template>
				</xsl:variable>

				<xsl:if test="count(exsl:node-set($qualifying-targets)/*) > 1">
					<xsl:call-template name="reportError">
						<xsl:with-param name="message" select="concat('Multiple (', count(exsl:node-set($qualifying-targets)/*), ') target elements were found in the graphic element for a single target/variant.')" />
					</xsl:call-template>
				</xsl:if>

				<xsl:if test="exsl:node-set($qualifying-targets)/bd:target">
					<xsl:variable name="target-element" select="exsl:node-set($qualifying-targets)/bd:target" />

					<xsl:apply-templates select="$target-element" mode="graphic">
						<xsl:with-param name="graphic-element" select="." />
					</xsl:apply-templates>
				</xsl:if>
			</xsl:when>

			<xsl:otherwise>
				<xsl:call-template name="drawGraphic">
					<xsl:with-param name="element-dpc" select="@dpc" />
					<xsl:with-param name="element-dpi" select="@dpi" />
					<xsl:with-param name="element-position" select="@position" />
					<xsl:with-param name="element-url" select="@url" />
					<xsl:with-param name="element-width" select="@width" />
					<xsl:with-param name="element-height" select="@height" />
					<xsl:with-param name="is-inline" select="parent::bd:para | parent::bd:entry" />
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="bd:target" mode="graphic">
		<xsl:param name="graphic-element" />

		<xsl:variable name="dpc">
			<xsl:choose>
				<xsl:when test="@dpc">
					<xsl:value-of select="@dpc" />
				</xsl:when>

				<xsl:otherwise>
					<xsl:value-of select="$graphic-element/@dpc" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="dpi">
			<xsl:choose>
				<xsl:when test="@dpi">
					<xsl:value-of select="@dpi" />
				</xsl:when>

				<xsl:otherwise>
					<xsl:value-of select="$graphic-element/@dpi" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="position">
			<xsl:choose>
				<xsl:when test="@position">
					<xsl:value-of select="@position" />
				</xsl:when>

				<xsl:otherwise>
					<xsl:value-of select="$graphic-element/@position" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="url">
			<xsl:choose>
				<xsl:when test="@url">
					<xsl:value-of select="@url" />
				</xsl:when>

				<xsl:otherwise>
					<xsl:value-of select="$graphic-element/@url" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="width">
			<xsl:choose>
				<xsl:when test="@width">
					<xsl:value-of select="@width" />
				</xsl:when>

				<xsl:otherwise>
					<xsl:value-of select="$graphic-element/@width" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="height">
			<xsl:choose>
				<xsl:when test="@height">
					<xsl:value-of select="@height" />
				</xsl:when>

				<xsl:otherwise>
					<xsl:value-of select="$graphic-element/@height" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:call-template name="drawGraphic">
			<xsl:with-param name="element-dpc" select="$dpc" />
			<xsl:with-param name="element-dpi" select="$dpi" />
			<xsl:with-param name="element-position" select="$position" />
			<xsl:with-param name="element-url" select="$url" />
			<xsl:with-param name="element-width" select="$width" />
			<xsl:with-param name="element-height" select="$height" />
			<xsl:with-param name="is-inline" select="$graphic-element/parent::bd:para | $graphic-element/parent::bd:entry" />
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="drawGraphic">
		<xsl:param name="element-dpc" />
		<xsl:param name="element-dpi" />
		<xsl:param name="element-position" />
		<xsl:param name="element-url" />
		<xsl:param name="element-width" />
		<xsl:param name="element-height" />
		<xsl:param name="is-inline" select="false()" />

		<xsl:variable name="dpcOrDpi">
			<xsl:choose>
				<xsl:when test="$element-dpc">
					<xsl:value-of select="'dpc'" />
				</xsl:when>

				<xsl:otherwise>
					<xsl:value-of select="'dpi'" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="dpx">
			<xsl:choose>
				<xsl:when test="$dpcOrDpi = 'dpc'">
					<xsl:value-of select="$element-dpc" />
				</xsl:when>

				<xsl:when test="$dpcOrDpi = 'dpi'">
					<xsl:choose>
						<xsl:when test="$element-dpi">
							<xsl:value-of select="$element-dpi" />
						</xsl:when>

						<xsl:otherwise>
							<xsl:value-of select="300" />
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="position">
			<xsl:choose>
				<xsl:when test="$is-inline">
					<xsl:value-of select="'inline'" />
				</xsl:when>

				<xsl:otherwise>
					<xsl:choose>
						<xsl:when test="$element-position">
							<xsl:if test="$element-position != 'left' and $element-position != 'right' and $element-position != 'center'">
								<xsl:message terminate="yes">Invalid value for position in graphical element:
									<xsl:value-of select="$element-position" />
								</xsl:message>
							</xsl:if>

							<xsl:value-of select="$element-position" />
						</xsl:when>

						<xsl:otherwise>
							<xsl:value-of select="'center'" />
						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="positionClass">
			<xsl:choose>
				<xsl:when test="$position = 'left'">
					<xsl:value-of select="'bdml-graphic-left'" />
				</xsl:when>

				<xsl:when test="$position = 'right'">
					<xsl:value-of select="'bdml-graphic-right'" />
				</xsl:when>

				<xsl:when test="$position = 'inline'">
					<xsl:value-of select="'bdml-graphic-inline'" />
				</xsl:when>

				<xsl:otherwise>
					<xsl:value-of select="'bdml-graphic-center'" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<img>
			<xsl:call-template name="copyClassAndIdAttributes">
				<xsl:with-param name="extraClasses" select="$positionClass" />
			</xsl:call-template>

			<xsl:attribute name="src">
				<xsl:value-of select="concat($relativeRoot, $element-url)" />
			</xsl:attribute>

			<xsl:if test="$element-width">
				<xsl:call-template name="resolveMeasurement">
					<xsl:with-param name="widthOrHeight" select="'width'" />
					<xsl:with-param name="value" select="$element-width" />
					<xsl:with-param name="dpcOrDpi" select="$dpcOrDpi" />
					<xsl:with-param name="dpx" select="$dpx" />
				</xsl:call-template>
			</xsl:if>

			<xsl:if test="$element-height">
				<xsl:call-template name="resolveMeasurement">
					<xsl:with-param name="widthOrHeight" select="'height'" />
					<xsl:with-param name="value" select="$element-height" />
					<xsl:with-param name="dpcOrDpi" select="$dpcOrDpi" />
					<xsl:with-param name="dpx" select="$dpx" />
				</xsl:call-template>
			</xsl:if>
		</img>
	</xsl:template>

	<!-- ************************ COMMENTS ************************ -->

	<xsl:template match="bd:comment">
		<div class="bdml-comment">
			<p>
				<span class="bdml-title" onclick="commentClick(this)">
					<xsl:if test="@creator">
						<xsl:value-of select="' '" />
						<xsl:value-of select="@creator" />
					</xsl:if>

					<xsl:value-of select="' '" />
					<xsl:value-of select="@title" />
				</span>
			</p>

			<div>
				<xsl:attribute name="class">
					<xsl:choose>
						<xsl:when test="@state = 'open'">
							<xsl:value-of select="'bdml-popup-shown'" />
						</xsl:when>

						<xsl:otherwise>
							<xsl:value-of select="'bdml-popup-hidden'" />
						</xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>

				<xsl:apply-templates select="bd:question | bd:remark" mode="comments" />
			</div>
		</div>
	</xsl:template>

	<xsl:template match="bd:question" mode="comments">
		<div class="bdml-comment-question">
			<xsl:call-template name="copyClassAndIdAttributes" />

			<xsl:if test="@creator">
				<span class="bdml-comment-question-creator"><xsl:value-of select="@creator" /> </span>
			</xsl:if>

			<xsl:apply-templates/>
		</div>

		<xsl:value-of select="$newline" />
	</xsl:template>

	<xsl:template match="bd:remark" mode="comments">
		<div class="bdml-comment-remark">
			<xsl:call-template name="copyClassAndIdAttributes" />

			<xsl:if test="@creator">
				<span class="bdml-comment-remark-creator"><xsl:value-of select="@creator" /> </span>
			</xsl:if>

			<xsl:apply-templates/>
		</div>

		<xsl:value-of select="$newline" />
	</xsl:template>

	<!-- ****************** FORMATTED XML BLOCKS ****************** -->

	<xsl:template match="bd:xml">
		<pre class="bdml-code bdml-xml">
			<xsl:apply-templates mode="xmlFirst" />
		</pre>
	</xsl:template>

	<xsl:template match="*" mode="xmlFirst">
		<xsl:apply-templates mode="xml" select=".">
			<xsl:with-param name="count" select="0" />
		</xsl:apply-templates>
	</xsl:template>

	<xsl:template match="bd:document" mode="xmlFirst">
		<span class="bdml-xml-elm"><xsl:value-of select="'&lt;document'" /></span>
		<xsl:value-of select="' '" />
		<span class="bdml-xml-att"><xsl:value-of select="'xmlns'" /></span>
		<xsl:value-of select="'='" />
		<span class="bdml-xml-str"><xsl:value-of select="'&quot;http://boradoc.org/1.0&quot;'" /></span>
		<span class="bdml-xml-elm"><xsl:value-of select="'&gt;&#xa;'" /></span>

		<xsl:apply-templates mode="xml">
			<xsl:with-param name="count" select="2" />
		</xsl:apply-templates>
		<span class="bdml-xml-elm"><xsl:value-of select="'&lt;/document&gt;&#xa;'" /></span>
	</xsl:template>

	<xsl:template match="text()" mode="xmlFirst">
		<xsl:variable name="normalised" select="normalize-space(.)" />

		<xsl:if test="string-length($normalised) > 0">
			<xsl:value-of select="concat('F', $normalised, 'G')" />
		</xsl:if>
	</xsl:template>

	<xsl:template match="*" mode="xml" priority="2">
		<xsl:param name="count" select="'BUG'" />

		<xsl:choose>
			<xsl:when test="not(node())">
				<!-- Closed element (contains just attributes) -->
				<xsl:call-template name="printSpaces"><xsl:with-param name="count" select="$count" /></xsl:call-template>
				<span class="bdml-xml-elm"><xsl:value-of select="concat('&lt;', local-name())" /></span>
				<xsl:apply-templates select="@*" mode="xml" />
				<span class="bdml-xml-elm"><xsl:value-of select="concat(' /&gt;', $newline)" /></span>
			</xsl:when>

			<xsl:otherwise>
				<!-- Open element (contains descendant nodes)-->

				<!-- Determine whether to print on a single or multiple lines according to content. -->
				<xsl:choose>
					<!-- Print on a single line. -->
					<xsl:when test="not(*)">
						<xsl:call-template name="printSpaces"><xsl:with-param name="count" select="$count" /></xsl:call-template>
						<span class="bdml-xml-elm"><xsl:value-of select="concat('&lt;', local-name())" /></span>
						<xsl:apply-templates select="@*" mode="xml" />
						<span class="bdml-xml-elm"><xsl:value-of select="'&gt;'" /></span>
						<xsl:apply-templates mode="xml">
							<xsl:with-param name="count" select="0" />
						</xsl:apply-templates>
						<span class="bdml-xml-elm"><xsl:value-of select="concat('&lt;/', local-name(), '&gt;', $newline)" /></span>
					</xsl:when>

					<xsl:otherwise>
						<!-- Print on multiple lines. -->
						<xsl:call-template name="printSpaces"><xsl:with-param name="count" select="$count" /></xsl:call-template>
						<span class="bdml-xml-elm"><xsl:value-of select="concat('&lt;', local-name())" /></span>
						<xsl:apply-templates select="@*" mode="xml" />
						<span class="bdml-xml-elm"><xsl:value-of select="concat('&gt;', $newline)" /></span>
						<xsl:apply-templates mode="xml">
							<xsl:with-param name="count" select="$count + 2" />
							<xsl:with-param name="printNewline" select="true()" />
						</xsl:apply-templates>
						<xsl:call-template name="printSpaces"><xsl:with-param name="count" select="$count" /></xsl:call-template>
						<span class="bdml-xml-elm"><xsl:value-of select="concat('&lt;/', local-name(), '&gt;', $newline)" /></span>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="comment()" mode="xml" priority="2">
		<xsl:param name="count" select="'BUG'" />
		<xsl:call-template name="printSpaces"><xsl:with-param name="count" select="$count" /></xsl:call-template>
		<span class="bdml-xml-cmt"><xsl:value-of select="concat('&lt;--', ., '--&gt;', $newline)" /></span>
	</xsl:template>

	<xsl:template match="@*" mode="xml">
		<xsl:value-of select="' '" />
		<span class="bdml-xml-att"><xsl:value-of select="name(.)" /></span>
		<xsl:value-of select="'='" />
		<span class="bdml-xml-str"><xsl:value-of select="concat('&quot;', ., '&quot;')" /></span>
	</xsl:template>

	<xsl:template match="node()" mode="xml" priority="1">
		<xsl:param name="count" select="'BUG'" />
		<xsl:param name="printNewline" select="false()" />

		<xsl:if test="$printNewline">
			<xsl:call-template name="printSpaces"><xsl:with-param name="count" select="$count" /></xsl:call-template>
		</xsl:if>

		<xsl:value-of select="." />

		<xsl:if test="$printNewline">
			<xsl:value-of select="$newline" />
		</xsl:if>

		<xsl:apply-templates mode="xml" />
	</xsl:template>

	<xsl:template match="text()" mode="xml" priority="2">
		<xsl:param name="count" select="'BUG'" />
		<xsl:param name="printNewline" select="false()" />

		<xsl:call-template name="printSpaces"><xsl:with-param name="count" select="$count" /></xsl:call-template>

		<xsl:value-of select="." />

		<xsl:if test="$printNewline">
			<xsl:value-of select="$newline" />
		</xsl:if>

	</xsl:template>

	<xsl:template name="printSpaces">
		<xsl:param name="count" select="'BUG'" />

		<xsl:choose>
			<xsl:when test="$count = 0">
			</xsl:when>

			<xsl:when test="$count = 1">
				<xsl:value-of select="' '" />
			</xsl:when>

			<xsl:otherwise>
				<xsl:value-of select="' '" />
				<xsl:call-template name="printSpaces">
					<xsl:with-param name="count" select="$count - 1" />
				</xsl:call-template>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<!-- *********************** CODE BLOCKS ********************** -->

	<xsl:template match="bd:code">
		<xsl:value-of select="$newline" />

		<pre>
			<xsl:attribute name="class">
				<xsl:choose>
					<xsl:when test="@lang">
						<xsl:choose>
							<xsl:when test="translate(@lang, $uppercase, $lowercase) = 'c++'">
								<xsl:value-of select="'bdml-code bdml-cpp'" />
							</xsl:when>

							<xsl:when test="translate(@lang, $uppercase, $lowercase) = 'properties'">
								<xsl:value-of select="'bdml-code bdml-properties'" />
							</xsl:when>

							<xsl:when test="translate(@lang, $uppercase, $lowercase) = 'c'">
								<xsl:value-of select="'bdml-code bdml-cpp'" />
							</xsl:when>

							<xsl:when test="translate(@lang, $uppercase, $lowercase) = 'java'">
								<xsl:value-of select="'bdml-code bdml-java'" />
							</xsl:when>

							<xsl:when test="translate(@lang, $uppercase, $lowercase) = 'xml'">
								<xsl:value-of select="'bdml-code bdml-xml'" />
							</xsl:when>

							<xsl:otherwise>
								<xsl:value-of select="concat('bdml-code bdml-', translate(@lang, $uppercase, $lowercase))" />
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>

					<xsl:otherwise>
						<xsl:value-of select="'bdml-code bdml-none'" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>

			<xsl:apply-templates mode="code" />
		</pre>
	</xsl:template>

	<xsl:template match="*" mode="code">
		<span class="bdml-{local-name()}">
			<xsl:apply-templates/>
		</span>
	</xsl:template>

	<!-- ******************* LANGUAGE GRAMMAR ********************* -->

	<!-- TODO replace with code element-->

	<xsl:template match="bd:grammar">
		<pre class="bdml-grammar">
			<xsl:call-template name="copyClassAndIdAttributes"/>
			<xsl:apply-templates mode="grammar"/>
		</pre>
	</xsl:template>

	<xsl:template match="*" mode="grammar">
		<span class="bdml-{local-name()}">
			<xsl:apply-templates mode="grammar"/>
		</span>
	</xsl:template>

	<!-- *************** VERBATIM XML COPY THROUGH **************** -->

	<xsl:template match="bd:verbatim">
		<xsl:if test="@target = $target">
			<xsl:apply-templates mode="verbatim" />
		</xsl:if>
	</xsl:template>

	<xsl:template match="@* | node()" mode="verbatim">
		<xsl:copy>
			<xsl:apply-templates select="@* | node()" mode="verbatim" />
		</xsl:copy>
	</xsl:template>

	<!-- *************** EXPLICITLY CALLED TEMPLATES ************** -->

	<xsl:template name="copyClassAndIdAttributes">
		<xsl:param name="extraClasses" select="''" />

		<xsl:choose>
			<xsl:when test="$extraClasses = ''">
				<xsl:if test="@class">
					<xsl:attribute name="class">
						<xsl:value-of select="@class" />
					</xsl:attribute>
				</xsl:if>
			</xsl:when>

			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="@class">
						<xsl:attribute name="class">
							<xsl:value-of select="concat($extraClasses, ' ', @class)" />
						</xsl:attribute>
					</xsl:when>

					<xsl:otherwise>
						<xsl:attribute name="class">
							<xsl:value-of select="$extraClasses" />
						</xsl:attribute>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>

		<xsl:if test="@id">
			<xsl:attribute name="id">
				<xsl:value-of select="@id" />
			</xsl:attribute>
		</xsl:if>
	</xsl:template>

	<xsl:template name="copyClassAttribute">
		<xsl:param name="extraClasses" select="''" />

		<xsl:choose>
			<xsl:when test="$extraClasses = ''">
				<xsl:if test="@class">
					<xsl:attribute name="class">
						<xsl:value-of select="@class" />
					</xsl:attribute>
				</xsl:if>
			</xsl:when>

			<xsl:otherwise>
				<xsl:if test="@class">
					<xsl:attribute name="class">
						<xsl:value-of select="concat($extraClasses, ' ', @class)" />
					</xsl:attribute>
				</xsl:if>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="applicable-targets-and-variants">
		<xsl:param name="elements" />

		<xsl:param name="theTarget" />

		<xsl:variable name="qualifying-elements">
			<xsl:for-each select="$elements">
				<xsl:variable name="element" select="." />

				<xsl:variable name="applies">
					<xsl:call-template name="target-and-variant-applies">
						<xsl:with-param name="target-list" select="@target" />
						<xsl:with-param name="variant-list" select="@variant" />
						<xsl:with-param name="theTarget" select="$theTarget" />
					</xsl:call-template>
				</xsl:variable>

				<xsl:if test="string($applies) = 'true'">
					<xsl:copy-of select="$element" />
				</xsl:if>
			</xsl:for-each>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="count(exsl:node-set($qualifying-elements)/*) > 1">
				<xsl:variable name="non-default-elements">
					<xsl:for-each select="exsl:node-set($qualifying-elements)/*">
						<xsl:if test="@target or @variant">
							<xsl:copy-of select="." />
						</xsl:if>
					</xsl:for-each>
				</xsl:variable>

				<xsl:choose>
					<xsl:when test="count(exsl:node-set($non-default-elements)/*) > 0">
						<xsl:copy-of select="$non-default-elements" />
					</xsl:when>

					<xsl:otherwise>
						<xsl:copy-of select="exsl:node-set($qualifying-elements)/*" />
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>

			<xsl:otherwise>
				<xsl:copy-of select="exsl:node-set($qualifying-elements)/*" />
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="target-and-variant-applies">
		<xsl:param name="target-list" />
		<xsl:param name="variant-list" />

		<xsl:param name="theTarget" />

		<xsl:variable name="target-valid">
			<xsl:choose>
				<xsl:when test="string-length($target-list) > 0">
					<xsl:variable name="target-tokens">
						<xsl:call-template name="tokenize-string">
							<xsl:with-param name="text" select="$target-list" />
							<xsl:with-param name="delimiter" select="' '" />
						</xsl:call-template>
					</xsl:variable>

					<xsl:for-each select="exsl:node-set($target-tokens)/*">
						<xsl:variable name="target-token" select="string(.)" />

						<xsl:if test="$target-token = $theTarget">
							<bd:para>true</bd:para>
						</xsl:if>
					</xsl:for-each>
				</xsl:when>

				<xsl:otherwise>
					<bd:para>true</bd:para>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="variant-valid">
			<xsl:choose>
				<xsl:when test="string-length($variant-list) > 0">
					<xsl:variable name="variant-tokens">
						<xsl:call-template name="tokenize-string">
							<xsl:with-param name="text" select="$variant-list" />
							<xsl:with-param name="delimiter" select="' '" />
						</xsl:call-template>
					</xsl:variable>

					<xsl:for-each select="exsl:node-set($variant-tokens)/*">
						<xsl:variable name="variant-token" select="string(.)" />

						<xsl:if test="$variant-token = $variant">
							<bd:para>true</bd:para>
						</xsl:if>
					</xsl:for-each>
				</xsl:when>

				<xsl:otherwise>
					<bd:para>true</bd:para>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:if test="count(exsl:node-set($target-valid)/bd:para) > 0 and count(exsl:node-set($variant-valid)/bd:para) > 0">
			<bd:para>true</bd:para>
		</xsl:if>
	</xsl:template>

	<xsl:template name="tokenize-string">
		<xsl:param name="text" />
		<xsl:param name="delimiter" />

		<xsl:variable name="lastOne" select="normalize-space($text)" />
		<xsl:variable name="first" select="normalize-space(substring-before($lastOne, $delimiter))" />
		<xsl:variable name="remaining" select="normalize-space(substring-after($lastOne, $delimiter))" />

		<xsl:choose>
			<xsl:when test="string-length($first) > 0">
				<bd:para><xsl:value-of select="$first" /></bd:para>

				<xsl:call-template name="tokenize-string">
					<xsl:with-param name="text" select="$remaining" />
					<xsl:with-param name="delimiter" select="$delimiter" />
				</xsl:call-template>
			</xsl:when>

			<xsl:when test="string-length($lastOne) > 0">
				<bd:para><xsl:value-of select="$lastOne" /></bd:para>
			</xsl:when>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="reportError">
		<xsl:param name="message" />

		<xsl:comment>
			<xsl:value-of select="concat('Error: ', $message)" />
		</xsl:comment>

		<xsl:message terminate="yes">
			<xsl:value-of select="concat('Error: ', $message)" />
		</xsl:message>
	</xsl:template>
</xsl:transform>
