<?xml version="1.0"?>
<xsl:transform version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
   <xsl:output method="text"/>
   <xsl:strip-space elements="*"/>

   <xsl:template match="Order">
      <xsl:for-each select ="*">
	 <xsl:value-of select="."/>
	 <xsl:if test="position() != last()">
	    <xsl:value-of select="','"/>
	 </xsl:if>
      </xsl:for-each>
      <xsl:text>&#10;</xsl:text>
   </xsl:template>

</xsl:transform>
