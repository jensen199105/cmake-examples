clc
clear
close all

LUDA_TOOL_VERSION = '1.0.0.1';

% resInfo = 'WB_2mic_28mm_1125_headset';

%% Attention: To be modified by user!
FLOAT_VERSION = 1; % 0 --> Fixed-point; 1 --> Float-point 

% WB_name = 'WB_2mic_150mm_0618'; % Select the name of WB mat file
% WB_name = 'WB_2mic_160mm_1203'; % Select the name of WB mat file
WB_name = 'WB_2mic_28mm_1125_headset';% Select the name of WB mat file

%% Judge the dimemsions for W and B matrix.
load(['./', WB_name, '.mat']);

BEAMS_OUTPUT_DIMS = 0;
if     (isequal(size(W), [2 257 3]) && (isequal(size(B), [2 257 3])))
    disp('Both the size of W and B are 2x257x3.');
    BEAMS_OUTPUT_DIMS = 3;
elseif (isequal(size(W), [2 129 3]) && (isequal(size(B), [2 129 3])))
    disp('Both the size of W and B are 2x129x3.');
    BEAMS_OUTPUT_DIMS = 3;
elseif (isequal(size(W), [2 257])   && (isequal(size(B), [2 257])))
    disp('Both the size of W and B are 2x257x1.');
    BEAMS_OUTPUT_DIMS = 1;
elseif (isequal(size(W), [2 129])   && (isequal(size(B), [2 129])))
    disp('Both the size of W and B are 2x129x1.');
    BEAMS_OUTPUT_DIMS = 1;
else
    disp('!Attention: The size of W and B is not supported!') 
    return;
end

%% Judge the amplitude for W and B matrix.
if FLOAT_VERSION == 0
    realAbsMax = max(max(max(max(abs(real(W))))), max(max(max(abs(real(B))))))
    imagAbsMax = max(max(max(max(abs(imag(W))))), max(max(max(abs(imag(B))))))
    if realAbsMax >= 127 || imagAbsMax >= 127
        disp('!The amplitude of W/B is out of range for fixed version W32Q24!');
    end
end

%% Generate 3-Beam WB C source file.
if BEAMS_OUTPUT_DIMS == 3
    [dim1, dim2, dim3] = size(W);
    
    if FLOAT_VERSION == 1
        fname = fopen(['LUDA_FLOAT_', WB_name, '_3EntBeam.c'], 'w');
    else
        fname = fopen(['LUDA_FIXED_', WB_name, '_3EntBeam.c'], 'w');
    end
    
    fprintf(fname, '/*\n');
    fprintf(fname, [' * LUDA MATWB2CFILE TOOL Version: ', LUDA_TOOL_VERSION, '\n']);
    fprintf(fname, ' * The W/B coefficients are arranged as below:\n');
    fprintf(fname, ' * \tmic1\tbeam1\t257x1\tcomplex\n');
    fprintf(fname, ' * \tmic2\tbeam1\t257x1\tcomplex\n');
    fprintf(fname, ' * \tmic1\tbeam2\t257x1\tcomplex\n');
    fprintf(fname, ' * \tmic2\tbeam2\t257x1\tcomplex\n');
    fprintf(fname, ' * \tmic1\tbeam3\t257x1\tcomplex\n');
    fprintf(fname, ' * \tmic2\tbeam3\t257x1\tcomplex\n');
    fprintf(fname, ' */\n');
    
    fprintf(fname, '\nchar acResInfo[] = "%s";\n\n', resInfo);
    
    if FLOAT_VERSION == 1
        fprintf(fname, 'const float g_asGscWq[] =\n{\n');
    else
        fprintf(fname, '/* W32Q24 */\nconst int g_asGscWq[] =\n{\n');
    end
    % W is interpreted as W32Q24
    for beam = 1:dim3
        for mics = 1:dim1
            fprintf(fname, '    /* mic%d beam%d */\n', mics, beam);
            for bins = 1:dim2
    if FLOAT_VERSION == 1
                fprintf(fname, '    %.10f,',   real(W(mics, bins, beam)));
                fprintf(fname, '    %.10f,\n', imag(W(mics, bins, beam)));
    else
                fprintf(fname, '    %d,',   round(real(W(mics, bins, beam)*2^24)));
                fprintf(fname, '    %d,\n', round(imag(W(mics, bins, beam)*2^24)));
    end
            end
        end
    end
    fprintf(fname, '};\n\n');

    if FLOAT_VERSION == 1
        fprintf(fname, 'const float g_asGscB[] =\n{\n');
    else
        fprintf(fname, '/* W32Q24 */\nconst int g_asGscB[] =\n{\n');
    end
    % B is interpreted as W32Q24
    for beam = 1:dim3
        for mics = 1:dim1
            fprintf(fname, '    /* mic%d beam%d */\n', mics, beam);
            for bins = 1:dim2
    if FLOAT_VERSION == 1
                fprintf(fname, '    %.10f,',   real(B(mics, bins, beam)));
                fprintf(fname, '    %.10f,\n', imag(B(mics, bins, beam)));
    else
                fprintf(fname, '    %d,',   round(real(B(mics, bins, beam)*2^24)));
                fprintf(fname, '    %d,\n', round(imag(B(mics, bins, beam)*2^24)));
    end
            end
        end
    end
    fprintf(fname, '};\n\n');

    fclose all;

%% Generate 1-Beam WB C source file.
elseif BEAMS_OUTPUT_DIMS == 1
    [dim1, dim2] = size(W);
    
    if FLOAT_VERSION == 1
        fname = fopen(['LUDA_FLOAT_', WB_name, '_1EntBeam.c'], 'w');
    else
        fname = fopen(['LUDA_FIXED_', WB_name, '_1EntBeam.c'], 'w');
    end

    fprintf(fname, '/*\n');
    fprintf(fname, [' * LUDA MATWB2CFILE TOOL Version: ', LUDA_TOOL_VERSION, '\n']);
    fprintf(fname, ' * The W/B coefficients are arranged as below:\n');
    fprintf(fname, ' * \tmic1\tbeam1\t257x1\tcomplex\n');
    fprintf(fname, ' * \tmic2\tbeam1\t257x1\tcomplex\n');
    fprintf(fname, ' */\n');
    
    fprintf(fname, '\nchar acResInfo[] = "%s";\n\n', resInfo);
    
    if FLOAT_VERSION == 1
        fprintf(fname, 'const float g_asGscWq[] =\n{\n');
    else
        fprintf(fname, '/* W32Q24 */\nconst int g_asGscWq[] =\n{\n');
    end
    % W is interpreted as W32Q24
    for beam = 1
        for mics = 1:dim1
            fprintf(fname, '    /* mic%d beam%d */\n', mics, beam);
            for bins = 1:dim2
    if FLOAT_VERSION == 1
                fprintf(fname, '    %.10f,',   real(W(mics, bins)));
                fprintf(fname, '    %.10f,\n', imag(W(mics, bins)));
    else
                fprintf(fname, '    %d,',   round(real(W(mics, bins)*2^24)));
                fprintf(fname, '    %d,\n', round(imag(W(mics, bins)*2^24)));
    end
            end
        end
    end
    fprintf(fname, '};\n\n');

    if FLOAT_VERSION == 1
        fprintf(fname, 'const float g_asGscB[] =\n{\n');
    else
        fprintf(fname, '/* W32Q24 */\nconst int g_asGscB[] =\n{\n');
    end
    % B is interpreted as W32Q24
    for beam = 1
        for mics = 1:dim1
            fprintf(fname, '    /* mic%d beam%d */\n', mics, beam);
            for bins = 1:dim2
    if FLOAT_VERSION == 1
                fprintf(fname, '    %.10f,',   real(B(mics, bins)));
                fprintf(fname, '    %.10f,\n', imag(B(mics, bins)));
    else
                fprintf(fname, '    %d,',   round(real(B(mics, bins)*2^24)));
                fprintf(fname, '    %d,\n', round(imag(B(mics, bins)*2^24)));
    end
            end
        end
    end
    fprintf(fname, '};\n\n');

    fclose all; 
end

