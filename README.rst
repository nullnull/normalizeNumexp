================================
normalizeNumexp : Numerical/Temporal expression normalizer
================================

About
=====

This is a tool for normalizing numerical/temporal expression.

Install
=======

Do following instructions.

..

  $ ./waf configure

  $ ./waf build

  $ ./waf install

To check that the installation has completed successfully, 

..

  $ ./waf --checkall


How to Use
=======

This utility normalize (Japanese) numerical and temporal expressions in the input sentence.

..

  $ normalizeNumexp

  魔女狩りは15世紀〜18世紀にかけてみられ、全ヨーロッパで4万人が処刑された
  >numerical*4万人*29*32*人*40000*40000*
  >abstime*15世紀〜18世紀*5*14*none*1401-XX-XX*1800-XX-XX*

If you want to know more detail about this tool, please read following documents.
http://www.cl.ecei.tohoku.ac.jp/~katsuma/software/normalizeNumexp/
=======
  $ ./waf --checkall

