import java.util.Scanner;
import jp.ac.tohoku.ecei.cl.numexp.*;

public class TestNormalizeNumexp {
    static {
        System.loadLibrary("normalize_numexp");
    }
    public static void main(String [] args) {
        NormalizeNumexp n = new NormalizeNumexp("ja");
        StringVector result = new StringVector(0);
        
        String text = "魔女狩りは15世紀〜18世紀にかけてみられ、全ヨーロッパで4万人が処刑された";
        n.normalize(text, result);

        System.out.println("text:" + text);
        for (long i = 0, size = result.size(); i < size; i++) {
            System.out.println(result.get((int)i));
        }
    }
}
