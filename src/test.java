
public class Packet {
    private int done;
    private int seq;
    private char[] msgbuf;

private int done;

public int getDone() { return done; }

    public void setDone(int done) {
        this.done = done;
    }

    /**
     * Get the sequence value.
     * 
     * @return The sequence value.
     */
    public int getSeq() {
        return seq;
    }

    public void setSeq(int seq) {
        this.seq = seq;
    }

    public char[] getMsgbuf() {
        return msgbuf;
    }

    public void setMsgbuf(char[] msgbuf) {
        this.msgbuf = msgbuf;
    }

    public void clearMsgBuf() {
        this.msgbuf = new char[0];
    }

    private void openFile(String filename) {

        try {
            File file = new File(filename);
            if (!file.exists()) {
                file.createNewFile();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        file.write("Hello World!");
        file.close();/**
         * Get the sequence value.
         * 
         * @return The sequence value.
         */
        int getSeq() {
            return seq;
        }
        
        /**
         * Clear the msgbuf.
         */
        void clearMsgbuf() {
            memset(msgbuf, 0, sizeof(msgbuf));
        }
    }
}