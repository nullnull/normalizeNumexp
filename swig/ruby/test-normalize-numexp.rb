#coding:utf-8
require "normalize_numexp"

n = Normalize_numexp::NormalizeNumexp::new("ja")
text = "魔女狩りは15世紀〜18世紀にかけてみられ、全ヨーロッパで4万人が処刑された"
result = Normalize_numexp::StringVector::new(0)

n.normalize(text, result)
print "text:#{text}\n"
result.each do |r|
		print "#{r}\n"
end
