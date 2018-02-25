import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.OptionGroup;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Vector;
import sorting.MergeSort;


public class MergeSortMain {
    /*
        Main class
    */

    public static void main(String[] args) throws IOException {
        /*
            Main method, it sets a parser, parse the input and run the tests
        */
        Integer[] array; // the array to be sorted
        CommandLine cmdLine = parse(args); // parsing the user input
        String file =  cmdLine.getOptionValue("file"); // the file in which the integers to sort are stored
        int numThread, executions; // max number of thread running at the same time, number of executions
        long avDuration = 0; // average duration of an execution

        numThread = cmdLine.hasOption("single") ?
                1 :
                Integer.parseInt(cmdLine.getOptionValue("multi"));
        executions = Integer.parseInt(cmdLine.getOptionValue("executions"));
        array = parseTestFile(file);

        for (int i=0; i<executions; ++i) {
            // for each execution
            Integer[] tmp;
            long start = System.currentTimeMillis(); // getting the time at the beginning
            tmp = new MergeSort<Integer>(array).sort(numThread); // running a test
            avDuration += System.currentTimeMillis()-start; // computing the ellapsed time
            if (!checkArray(tmp)) // checking the array (just to be sure)
                System.err.println("Array not sorted !");
            //else
            //    writeResult(file, tmp);
        }
        avDuration /= executions; // so far avDuration was a sum of ellapsed time, making it the average duration

        System.out.println(String.format( // printing the results
                "%d executions completed\n\tAverage duration of %d ms\n\t%d threads used\n\tArray of %d integers",
                executions,
                avDuration,
                numThread,
                array.length
        ));
    }

    private static CommandLine parse(final String[] args) {
        // Function setting a parser and parsing the user input
        Options options = new Options();
        OptionGroup group = new OptionGroup();
        Option option = new Option(
                "s",
                "single",
                false,
                "Using single thread mode"
        );
        group.addOption(option);
        option = new Option(
                "m",
                "multi",
                true,
                "Using multi thread mode"
        );
        group.addOption(option);
        group.setRequired(true);
        options.addOptionGroup(group);
        option = new Option(
                "f",
                "file",
                true,
                "File containing the integers to sort"
        );
        option.setRequired(true);
        options.addOption(option);
        option = new Option(
                "e",
                "executions",
                true,
                "The number of executions to be run"
        );
        option.setRequired(false);
        options.addOption(option);

        CommandLineParser parser = new DefaultParser();
        HelpFormatter formatter = new HelpFormatter();
        CommandLine cmdLine = null;

        try {
            cmdLine = parser.parse(options, args);
        } catch (ParseException e) {
            System.out.println(e.getMessage());
            formatter.printHelp("utility-name", options);
            System.exit(1);
        }

        return cmdLine;
    }

    private static Integer[] parseTestFile(final String filename) throws IOException {
        /*
            Function used to parse a file containing integer (one positive integer per line)
        */
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        Vector<Integer> tmp = new Vector<>();
        String line = reader.readLine();
        while (line != null) {
            tmp.add(Integer.parseInt(line));
            line = reader.readLine();
        }
        reader.close();
        Integer[] res = new Integer[tmp.size()];
        for (int i=0; i<tmp.size(); ++i)
            res[i] = tmp.get(i);
        return res;
    }

    private static boolean checkArray(final Integer[] array) {
        // Fuction checking if the array is sorted
        boolean flag = true;
        int i = 1;
        while (flag && i<array.length)
            flag = array[i-1] <= array[i++];
        return flag;
    }

    private static void writeResult(final String file, final Integer[] array) throws IOException {
        // This function is never used, it was meant to write the sorted integers into a file
        BufferedWriter writer = new BufferedWriter(new FileWriter(file));
        for (Integer i : array) {
            String tmp = i.toString();
            writer.write(tmp, 0, tmp.length());
            writer.newLine();
        }
        writer.flush();
        writer.close();
    }
}
