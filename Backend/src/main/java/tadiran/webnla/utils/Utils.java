package tadiran.webnla.utils;


import com.google.gson.*;

import com.google.gson.reflect.TypeToken;
import com.google.gson.stream.JsonReader;
import com.google.gson.stream.JsonWriter;
import lombok.ToString;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.hibernate.annotations.processing.Exclude;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;


import jakarta.servlet.http.HttpServletRequest;
import java.io.IOException;
import java.lang.reflect.Type;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.time.LocalDate;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeFormatterBuilder;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Consumer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import static java.time.temporal.ChronoField.HOUR_OF_DAY;
import static java.time.temporal.ChronoField.MINUTE_OF_HOUR;

public class Utils {
    private static final Logger logger = LogManager.getLogger(Utils.class);

    public Gson gson = new GsonBuilder()
            .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
            .create();

    public Gson g = new GsonBuilder()
            .create();

    public static String toFlatJson(String str) {
        return flatJsonField(str, "id");
    }

    public static String toFlatJson(String json, String fieldName) {
        return flatJsonField(json, fieldName);
    }

    public <T> String toFlatJson(T obj) {
        return toFlatJson(gExclusion.toJson((T)obj));
    }

    public <T> String toFlatShortJson(T obj) {
        return toFlatJson(gShortExclusion.toJson((T)obj));
    }

    public <T> String toJson(T obj) {
        return gExclusion.toJson((T)obj);
    }

    public <T> String toShortJson(T obj) {
        return gShortExclusion.toJson((T)obj);
    }

    public <T> T fromJson(String json, Type classOfT) {
        String fixedJson = json.replace("\"undefined\"","null");
        //fixedJson = fixedJson.replace("daily_hour: \\\"(.)(.):(.)(.):..","daily_hour: \\\"$1$2:$3$4:..\"");
        try {
            return gExclusion.fromJson((String)fixedJson, (Type)classOfT);
        } catch (Exception e) {
            logger.error("json:  " + json + "\n json-error:  " + e);
        }
        return null;
    }

    public <T> T fromShortJson(String json, Type classOfT) {
        String fixedJson = json.replace("\"undefined\"","null");
        try {
            return gShortExclusion.fromJson((String)fixedJson, (Type)classOfT);
        } catch (Exception e) {
            logger.error("json:  " + json + "\n json-error:  " + e);
        }
        return null;
    }

    public <T> T fromGson(String json) {
        if (json == null) {
            return null;
        }
        Type typeOfT = new TypeToken<Class<T>>(){}.getType();
        return gExclusion.fromJson(json, typeOfT);

    }

    public Gson gExclusion = new GsonBuilder()
            .addSerializationExclusionStrategy(new ExclusionStrategy() {
                @Override
                public boolean shouldSkipClass(Class<?> clazz) {
                    return false;
                }

                @Override
                public boolean shouldSkipField(FieldAttributes field) {
                    return (field.getAnnotation(Exclude.class) != null) ;
                }
            })
            .addDeserializationExclusionStrategy(new ExclusionStrategy() {
                @Override
                public boolean shouldSkipClass(Class<?> clazz) {
                    return false;
                }

                @Override
                public boolean shouldSkipField(FieldAttributes field) {
                    return (field.getAnnotation(Exclude.class) != null) ;
                }
            })
            .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
            .registerTypeAdapter(LocalTime.class, new LocalTimeAdapter().nullSafe())
            .registerTypeAdapter(LocalDate.class, new LocalDateAdapter().nullSafe())
            .create();

    public Gson gShortExclusion = new GsonBuilder()
            .addSerializationExclusionStrategy(new ExclusionStrategy() {
                @Override
                public boolean shouldSkipClass(Class<?> clazz) {
                    return false;
                }

                @Override
                public boolean shouldSkipField(FieldAttributes field) {
                    return (field.getAnnotation(Exclude.class) != null) ;
                }
            })
            .addDeserializationExclusionStrategy(new ExclusionStrategy() {
                @Override
                public boolean shouldSkipClass(Class<?> clazz) {
                    return false;
                }

                @Override
                public boolean shouldSkipField(FieldAttributes field) {
                    return (field.getAnnotation(Exclude.class) != null) ;
                }
            })
            .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
            .registerTypeAdapter(LocalTime.class, new LocalShortTimeAdapter().nullSafe())
            .registerTypeAdapter(LocalDate.class, new LocalDateAdapter().nullSafe())
            .create();

    private static final ExclusionStrategy strategy = new ExclusionStrategy() {
        @Override
        public boolean shouldSkipClass(Class<?> clazz) {
            return false;
        }

        @Override
        public boolean shouldSkipField(FieldAttributes field) {
            return (field.getAnnotation(Exclude.class) != null) ;
        }
    };


    @FunctionalInterface
    public interface BiConsumer<T, U> {
        void accept(T t, U u);
    }

    public static <T> Consumer<T> withCounter(BiConsumer<Integer, T> consumer) {
        return withCounter(1, consumer);
    }

    public static <T> Consumer<T> withCounter(Integer idxStart, BiConsumer<Integer, T> consumer) {
        AtomicInteger counter = new AtomicInteger(idxStart);
        return item -> consumer.accept(counter.getAndIncrement(), item);
    }


    /**
     * Method 1
     * To merge two sets
     * using DoubleBrace Initialisation
     */
    public static <T> Set<T> mergeSet(Set<T> a, Set<T> b) {
        //if (a instanceof Stream) a = a.collect(Collectors.toSet());


        // Adding all elements of respective Sets
        // using addAll() method
        return new HashSet<T>() {
            {
                addAll(a);
                addAll(b);
            }
        };
    }

    public static <T> Set<T> mergeSet(Stream<T> a, Set<T> b) {
        return mergeSet(a.collect(Collectors.toSet()), b);
    }

    public static <T> Set<T> mergeSet(Set<T> a, Stream<T> b) {
        return mergeSet(a, b.collect(Collectors.toSet()));
    }

    public static <T> Set<T> mergeSet(Stream<T> a, Stream<T> b) {
        return mergeSet(a.collect(Collectors.toSet()), b.collect(Collectors.toSet()));
    }

    /**
     * Finds the first matching close brace - assuming an open brace has just been removed from the `start` position.
     */
    private static int closeBrace(StringBuilder s, int start) {
        int count = 1;
        boolean inQuotes = false;
        for (int i = start; i < s.length(); i++) {
            char ch = s.charAt(i);
            // Special case escapes.
            if (ch != '\\') {
                switch (ch) {
                    case '"':
                        inQuotes = !inQuotes;
                        break;
                    case '{':
                        if (!inQuotes) {
                            count += 1;
                        }
                        break;
                    case '}':
                        if (!inQuotes) {
                            count -= 1;
                            if (count == 0) {
                                return i;
                            }
                        }
                        break;
                }
            } else {
                // Escape character - skip the next character.
                if (i < s.length()) {
                    i += 1;
                }
            }
        }
        // Failed to find
        return s.length();
    }

    /**
     * Removes the JSON specified.
     */
    public static String flatJsonField(String json, String remove) {
        // Transfer to an sb for slicing and dicing.
        StringBuilder s = new StringBuilder(json);
        // Build my pattern
        Pattern p = Pattern.compile("\"" + remove + "\"\\s*:\\s*\\{");
        // Make my Matchjer.
        Matcher m = p.matcher(s);
        // Is it there?
        while (m.find()) {
            int start = m.start();
            int end = m.end();
            // Kill the match.
            s.delete(start, end);
            // Walk forward to find the close brace.
            end = closeBrace(s, start);
            // And remove it.
            if (end < s.length()) {
                s.delete(end, end + 1);
            }
            // Rebuild the matcher.
            m = p.matcher(s);
        }
        return s.toString();
    }


    public static final class LocalTimeAdapter extends TypeAdapter<LocalTime> {
        @Override
        public void write( final JsonWriter jsonWriter, final LocalTime localTime ) throws IOException {
            jsonWriter.value(localTime.format(DateTimeFormatter.ISO_LOCAL_TIME));
        }
        @Override
        public LocalTime read( final JsonReader jsonReader ) throws IOException {
            return LocalTime.parse(jsonReader.nextString());
        }
    }

    public static final class LocalShortTimeAdapter extends TypeAdapter<LocalTime> {
        @Override
        public void write( final JsonWriter jsonWriter, final LocalTime localTime ) throws IOException {
            jsonWriter.value(localTime.format(new DateTimeFormatterBuilder()
                    .appendValue(HOUR_OF_DAY, 2)
                    .appendLiteral(':')
                    .appendValue(MINUTE_OF_HOUR, 2)
                    .toFormatter()
            ));
        }
        @Override
        public LocalTime read( final JsonReader jsonReader ) throws IOException {
            return LocalTime.parse(jsonReader.nextString());
        }
    }

    public static final class LocalDateAdapter extends TypeAdapter<LocalDate> {
        @Override
        public void write( final JsonWriter jsonWriter, final LocalDate localDate ) throws IOException {
            jsonWriter.value(localDate.format(DateTimeFormatter.ISO_LOCAL_DATE));
        }
        @Override
        public LocalDate read( final JsonReader jsonReader ) throws IOException {
            return LocalDate.parse(jsonReader.nextString());
        }
    }

    public static String atob(String str) {
        String coded = Base64.getEncoder().encodeToString(str.getBytes());
        return coded;
    }

    public static String btoa(String coded) {
        byte[] decodedBytes = Base64.getDecoder().decode(coded);
        return new String(decodedBytes);
    }

    private static final String[] HEADERS_TO_TRY = {
            "X-Forwarded-For",
            "Proxy-Client-IP",
            "WL-Proxy-Client-IP",
            "HTTP_X_FORWARDED_FOR",
            "HTTP_X_FORWARDED",
            "HTTP_X_CLUSTER_CLIENT_IP",
            "HTTP_CLIENT_IP",
            "HTTP_FORWARDED_FOR",
            "HTTP_FORWARDED",
            "HTTP_VIA",
            "REMOTE_ADDR" };

    public static String getClientIpAddress(HttpServletRequest request) {
        for (String header : HEADERS_TO_TRY) {
            String ip = request.getHeader(header);

            if (ip != null && !ip.isEmpty() && !"unknown".equalsIgnoreCase(ip)) {
                if ("0:0:0:0:0:0:0:1".equals(ip))
                    return "127.0.0.1";
                else
                    return ip;
            }
        }

        return request.getRemoteAddr();
    }

    public static String verifyRequesterIP() {
        HttpServletRequest request =
                ((ServletRequestAttributes) Objects.requireNonNull(
                        RequestContextHolder.getRequestAttributes())).getRequest();

        String agentIp = Utils.getClientIpAddress(request);

        InetAddress IP= null;
        try {
            IP = InetAddress.getLocalHost();
            agentIp = (agentIp.equals("0:0:0:0:0:0:0:1") ? IP.getHostAddress() : agentIp );
        } catch (UnknownHostException e) {
            throw new RuntimeException(e);
        }

        return agentIp;
    }
}



