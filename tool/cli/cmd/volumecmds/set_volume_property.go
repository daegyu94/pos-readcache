package volumecmds

import (
	"errors"
	"fmt"

	"cli/cmd/displaymgr"
	"cli/cmd/globals"
	"cli/cmd/grpcmgr"
	pb "kouros/api"

	"github.com/spf13/cobra"
	"google.golang.org/protobuf/encoding/protojson"
)

var SetVolumePropertyCmd = &cobra.Command{
	Use:   "set-property [flags]",
	Short: "Set the properties of a volume.",
	Long: `
Set the properties of a volume.

Syntax: 
	poseidonos-cli volume set-property (--volume-name | -v) VolumeName 
	(--array-name | -a) ArrayName (--new-volume-name | -n) VolumeName
	[--primary-volume] [--secondary-volume]

Example: 
	poseidonos-cli volume set-property --volume-name Volume0 --array-name volume0 
	--new-volume-name NewVolume0 --primary-volume
`,

	RunE: func(cmd *cobra.Command, args []string) error {

		reqParam, buildErr := buildSetVolumePropertyReqParam()
		if buildErr != nil {
			fmt.Printf("failed to build request: %v", buildErr)
			return buildErr
		}

		posMgr, err := grpcmgr.GetPOSManager()
		if err != nil {
			fmt.Printf("failed to connect to POS: %v", err)
			return err
		}
		res, req, gRpcErr := posMgr.VolumeProperty(reqParam)

		reqJson, err := protojson.MarshalOptions{
			EmitUnpopulated: true,
		}.Marshal(req)
		if err != nil {
			fmt.Printf("failed to marshal the protobuf request: %v", err)
			return err
		}
		displaymgr.PrintRequest(string(reqJson))

		if gRpcErr != nil {
			globals.PrintErrMsg(gRpcErr)
			return gRpcErr
		}

		printErr := displaymgr.PrintProtoResponse(req.Command, res)
		if printErr != nil {
			fmt.Printf("failed to print the response: %v", printErr)
			return printErr
		}

		return nil
	},
}

func buildSetVolumePropertyReqParam() (*pb.SetVolumePropertyRequest_Param, error) {

	if globals.IsValidVolName(set_volume_property_volumeName) == false {
		err := errors.New("The volume name must contain [a-zA-Z0-9_- ] only.")
		return nil, err
	}

	if (set_volume_property_isprimaryvol) && (set_volume_property_issecondaryvol) {
		err := errors.New("A volume must be a primary volume or a secondary volume.")
		return nil, err
	}

	var (
		updateprimaryvol = false
		isprimaryvol     = false
	)

	if set_volume_property_isprimaryvol {
		updateprimaryvol = true
		isprimaryvol = true
	}

	if set_volume_property_issecondaryvol {
		updateprimaryvol = true
		isprimaryvol = false
	}

	param := &pb.SetVolumePropertyRequest_Param{
		Name:             set_volume_property_volumeName,
		Array:            set_volume_property_arrayName,
		NewVolumeName:    set_volume_property_newVolumeName,
		Updateprimaryvol: updateprimaryvol,
		Isprimaryvol:     isprimaryvol,
	}

	return param, nil
}

// Note (mj): In Go-lang, variables are shared among files in a package.
// To remove conflicts between variables in different files of the same package,
// we use the following naming rule: filename_variablename. We can replace this if there is a better way.
var (
	set_volume_property_volumeName     = ""
	set_volume_property_arrayName      = ""
	set_volume_property_newVolumeName  = ""
	set_volume_property_isprimaryvol   = false
	set_volume_property_issecondaryvol = false
)

func init() {
	SetVolumePropertyCmd.Flags().StringVarP(&set_volume_property_volumeName,
		"volume-name", "v", "",
		"The name of the volume to set the property.")
	SetVolumePropertyCmd.MarkFlagRequired("volume-name")

	SetVolumePropertyCmd.Flags().StringVarP(&set_volume_property_arrayName,
		"array-name", "a", "",
		"The name of the array where the volume belongs to.")
	SetVolumePropertyCmd.MarkFlagRequired("array-name")

	SetVolumePropertyCmd.Flags().StringVarP(&set_volume_property_newVolumeName,
		"new-volume-name", "n", "",
		"The new name of the volume.")

	SetVolumePropertyCmd.Flags().BoolVarP(&set_volume_property_isprimaryvol,
		"primary-volume", "", false,
		"If specified, the volume will be set to a primary volume. This flag cannot be set with --secondary-volume")

	SetVolumePropertyCmd.Flags().BoolVarP(&set_volume_property_issecondaryvol,
		"secondary-volume", "", false,
		"If specified, the volume will be set to a secondary volume for HA.")
}
