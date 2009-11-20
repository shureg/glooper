<?xml version="1.0"?>
<xsl:transform version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
   <xsl:output method="text"/>

   <xsl:template match="batch/run/step/trade">
      <xsl:value-of select="registration_timer"/>,
      <xsl:value-of select="price"/>,
      <xsl:value-of select="quantity"/>,
      <xsl:value-of select="batch_id"/>,
      <xsl:value-of select="run_id"/>,
      <xsl:value-of select="step_id"/>
   </xsl:template>

</xsl:transform>
