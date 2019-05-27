#!/bin/bash
sourceDir=$1
destQRC=$2
echo '<RCC>' > $destQRC
echo '    <qresource prefix="/">' >> $destQRC
find $sourceDir -type f| sed 's|^release/||'|gawk '{print "        <file>"$0"</file>";}' >>$destQRC
echo '    </qresource>' >> $destQRC
echo '</RCC>' >> $destQRC


