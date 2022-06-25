package kl.utility;

import java.io.*;
import java.net.URL;
import java.util.Arrays;

public abstract class Web {
    public static String downloadData(String url) throws IOException {
        final InputStream in = new URL(url).openConnection().getInputStream();
        byte[] mainBuff = new byte[0]; int bytesRead;
        final byte[] tempBuff = new byte[1024 * 1024];
        while ((bytesRead = in.read(tempBuff)) > 0) {
            final int lastLen = mainBuff.length;
            mainBuff = Arrays.copyOf(mainBuff, mainBuff.length + bytesRead);
            System.arraycopy(tempBuff, 0, mainBuff, lastLen, bytesRead);
        }
        return new String(mainBuff);
    }
}
